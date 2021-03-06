/**
 * \file WANDDocumentIterator.h
 * \brief
 * \date Feb 29, 2012
 * \author Xin Liu
 */

#ifndef _WAND_DOCUMENT_ITERATOR_H_
#define _WAND_DOCUMENT_ITERATOR_H_

#include "DocumentIterator.h"
#include "TermDocumentIterator.h"
#include "common/type_defs.h"
#include <ranking-manager/RankingManager.h>
#include <ranking-manager/RankDocumentProperty.h>

#include <ir/index_manager/utility/PriorityQueue.h>

#include <vector>
#include <map>
#include <boost/thread.hpp>

namespace sf1r
{

class WANDDocumentIterator : public DocumentIterator
{
    typedef UpperBoundInProperties::const_iterator property_name_term_index_iterator;
    typedef ID_FREQ_MAP_T::const_iterator term_index_ub_iterator;

public:
    WANDDocumentIterator();
    
    virtual ~WANDDocumentIterator();

public:

    void add(DocumentIterator* pDocIterator); 

    void setUB(bool useOriginalQuery, UpperBoundInProperties& ubmap);
    
    float getUB();
    
    const char* getProperty()
    {
        if (docIteratorList_.begin() == docIteratorList_.end())
            return NULL;
        return (*docIteratorList_.begin())->getProperty();
    }

    void initThreshold(float threshold);

    void setThreshold (float realThreshold);

    bool next();

    docid_t doc()
    {
        return currDoc_;
    }

    inline void doc_item(RankDocumentProperty& rankDocumentProperty, unsigned propIndex = 0);

    void df_cmtf(
        DocumentFrequencyInProperties& dfmap,
        CollectionTermFrequencyInProperties& ctfmap,
        MaxTermFrequencyInProperties& maxtfmap);


    count_t tf()
    {
        return 0;
    }

    bool empty()
    {
        return docIteratorList_.empty();
    }
    
    void queryBoosting(double& score, double& weight);

#if SKIP_ENABLED
    docid_t skipTo(docid_t target);

protected:
    docid_t do_skipTo(docid_t target);
#endif

protected:

    void initDocIteratorSorter();

    bool do_next();

    bool findPivot();

    bool processPrePostings(docid_t target);

protected:

    std::vector<DocumentIterator*> docIteratorList_;

    std::multimap<docid_t, DocumentIterator*> docIteratorSorter_;

    docid_t currDoc_;

    float currThreshold_;

    docid_t pivotDoc_;

    boost::mutex mutex_;
};

inline void WANDDocumentIterator::doc_item(
    RankDocumentProperty& rankDocumentProperty,
    unsigned propIndex)
{
    DocumentIterator* pEntry;
    std::vector<DocumentIterator*>::iterator iter = docIteratorList_.begin();
    for (; iter != docIteratorList_.end(); ++iter)
    {
        pEntry = (*iter);
        if (pEntry && pEntry->isCurrent())
            pEntry->doc_item(rankDocumentProperty,propIndex);
    }
}

}

#endif /* _WAND_DOCUMENT_ITERATOR_H_ */
