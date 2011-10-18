#ifndef SF1R_PRODUCTMANAGER_COLLECTIONPRODUCTDATASOURCE_H
#define SF1R_PRODUCTMANAGER_COLLECTIONPRODUCTDATASOURCE_H

#include <sstream>
#include <common/type_defs.h>


#include "pm_def.h"
#include "pm_types.h"
#include "pm_config.h"
#include "product_data_source.h"
#include <configuration-manager/PropertyConfig.h>

namespace sf1r
{

class DocumentManager;
class IndexManager;
    
class CollectionProductDataSource : public ProductDataSource
{
public:
    
    CollectionProductDataSource(const boost::shared_ptr<DocumentManager>& document_manager, const boost::shared_ptr<IndexManager>& index_manager, const PMConfig& config, const std::set<PropertyConfig, PropertyComp>& schema);
    
    ~CollectionProductDataSource();
    
    bool GetDocument(uint32_t docid, PMDocumentType& doc);
    
    void GetDocIdList(const izenelib::util::UString& uuid, std::vector<uint32_t>& docid_list, uint32_t exceptid);
    
    bool UpdateUuid(const std::vector<uint32_t>& docid_list, const izenelib::util::UString& uuid);
    
private:
    boost::shared_ptr<DocumentManager> document_manager_;
    boost::shared_ptr<IndexManager> index_manager_;
    PMConfig config_;
    std::set<PropertyConfig, PropertyComp> schema_;
};

}

#endif

