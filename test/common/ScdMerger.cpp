#include <common/ScdParser.h>
#include <common/ScdWriter.h>
#include <document-manager/Document.h>
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/lexical_cast.hpp>
#include <am/luxio/BTree.h>
#include <am/tc/BTree.h>
#include <am/leveldb/Table.h>
#include <am/range/AmIterator.h>
#include <am/sequence_file/ssfr.h>
using namespace sf1r;
namespace bfs = boost::filesystem;
using namespace std;



class ScdMerger
{
    
    struct ValueType
    {
        Document doc;
        int type;
        uint32_t n;
        
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & doc & type & n;
        }
        
        bool operator<(const ValueType& other) const
        {
            return n<other.n;
        }
    };
    
    
public:
    ScdMerger(const std::string& work_dir): work_dir_(work_dir)
    {
        bfs::remove_all(work_dir_);
        bfs::create_directories(work_dir_);
        
    }
    
    void Merge(const std::string& scdPath)
    {
        std::vector<string> propertyNameList;
        propertyNameList.push_back("DOCID");
        propertyNameList.push_back("Url");
        propertyNameList.push_back("Title");
        propertyNameList.push_back("Picture");
        propertyNameList.push_back("Price");
        propertyNameList.push_back("Source");
        propertyNameList.push_back("Category");
        propertyNameList.push_back("Attribute");
        propertyNameList.push_back("Content");
        propertyNameList.push_back("uuid");
        std::string working_file = work_dir_+"/working";
        bfs::remove_all(working_file);
        izenelib::am::ssf::Writer<> writer(working_file);
        writer.Open();
        std::vector<std::string> scdList;
        ScdParser parser(izenelib::util::UString::UTF_8);
        static const bfs::directory_iterator kItrEnd;
        for (bfs::directory_iterator itr(scdPath); itr != kItrEnd; ++itr)
        {
            if (bfs::is_regular_file(itr->status()))
            {
                std::string fileName = itr->path().filename();
                if (parser.checkSCDFormat(fileName))
                {
                    scdList.push_back(itr->path().string());
                    parser.load(scdPath+fileName);
                }
                else
                {
                    std::cout << "SCD File not valid " << fileName<<std::endl;
                }
            }
        }
        sort(scdList.begin(), scdList.end(), ScdParser::compareSCD);
        vector<string>::iterator scd_it;
        std::size_t n = 0;
        std::size_t nscd = 0;
        std::cout<<"Total SCD count : "<<scdList.size()<<std::endl;
        for (scd_it = scdList.begin(); scd_it != scdList.end(); scd_it++)
        {
            nscd++;
            if(nscd%10==0)
            {
                std::cout<<"Process "<<nscd<<" Scds"<<std::endl;
            }
            size_t pos = scd_it ->rfind("/")+1;
            string filename = scd_it ->substr(pos);
            int type = 1;
            std::cout << "Processing SCD file. " << bfs::path(*scd_it).stem() <<std::endl;

            switch (parser.checkSCDType(*scd_it))
            {
            case INSERT_SCD:
            {
                type = 1;

            }
            break;
            case DELETE_SCD:
            {
                type = 3;
            }
            break;
            case UPDATE_SCD:
            {
                type = 2;
            }
            break;
            default:
                break;
            }
            parser.load(*scd_it);
            for (ScdParser::iterator doc_iter = parser.begin(propertyNameList); doc_iter != parser.end(); ++doc_iter, ++n)
            {
                if(n%1000==0)
                {
                    std::cout<<"## Process "<<n<<" scd docs"<<std::endl; 
                }
                ValueType value;
                value.n = n;
                value.type = type;
                SCDDocPtr doc = (*doc_iter);
                value.doc = getDoc_(*doc);
                izenelib::util::UString docname = value.doc.property("DOCID").get<izenelib::util::UString>();
                if(docname.length()==0) continue;
                
                uint64_t hash = izenelib::util::HashFunction<izenelib::util::UString>::generateHash64(docname);
                
                writer.Append(hash, value);
                std::string str_docname;
                docname.convertString(str_docname, izenelib::util::UString::UTF_8);
            }
        }
        
        writer.Close();
        izenelib::am::ssf::Sorter<uint32_t, uint64_t>::Sort(working_file);
        izenelib::am::ssf::Joiner<uint32_t, uint64_t, ValueType> joiner(working_file);
        joiner.Open();
        ScdWriter scd_writer("./", INSERT_SCD);
        uint64_t key;
        std::vector<ValueType> value_list;
        
        while( joiner.Next(key, value_list) )
        {
            std::sort(value_list.begin(), value_list.end());
            Document document;
            for( uint32_t i=0;i<value_list.size();i++)
            {
                ValueType& value = value_list[i];
                if(value.type==3)
                {
                    document.clear();
                }
                else if(!document.hasProperty("DOCID"))
                {
                    document = value.doc;
                }
                else
                {
                    //document already exists, and type==I or U
                    if(value.type==2)
                    {
                        document.copyPropertiesFromDocument(value.doc, true);
                    }
                }
            }
            if(document.hasProperty("DOCID"))
            {
                scd_writer.Append(document);
            }
        }
        scd_writer.Close();
        bfs::remove_all(working_file);
    }
    
    Document getDoc_(SCDDoc& doc)
    {
        Document result;
        vector<pair<izenelib::util::UString, izenelib::util::UString> >::iterator p;
        std::string fieldStr;
        for (p = doc.begin(); p != doc.end(); p++)
        {
            p->first.convertString(fieldStr, izenelib::util::UString::UTF_8);
            result.property(fieldStr) = p->second;
        }
        return result;
    }
    
private:
    std::string work_dir_;
    
    
};



int main(int argc, char** argv)
{
    std::string scdPath = argv[1];
    
    ScdMerger merger("./scd_merger_workdir");
    merger.Merge(scdPath);
}


