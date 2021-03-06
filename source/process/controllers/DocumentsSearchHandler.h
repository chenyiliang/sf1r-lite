#ifndef PROCESS_CONTROLLERS_DOCUMENTS_SEARCH_HANDLER_H
#define PROCESS_CONTROLLERS_DOCUMENTS_SEARCH_HANDLER_H
/**
 * @file process/controllers/DocumentsSearchHandler.h
 * @author Ian Yang
 * @date Created <2010-06-04 13:44:02>
 */
#include <common/inttypes.h> // docid_t

#include "CollectionHandler.h"

#include <renderers/DocumentsRenderer.h>

#include <util/driver/Request.h>
#include <util/driver/Response.h>
#include <util/driver/Value.h>

#include <query-manager/ActionItem.h> // KeywordSearchActionItem
#include <common/ResultType.h>

#include <vector>
#include <string>

namespace sf1r
{

class DocumentsSearchHandler
{
    static const int kRefinedQueryThreshold;

public:
    DocumentsSearchHandler(
        ::izenelib::driver::Request& request,
        ::izenelib::driver::Response& response,
        const CollectionHandler& collectionHandler
    );

    /// @internal
    /// @brief handler documents/search
    void search();

    /// @internal
    /// @brief parse request
    bool parse();

    const KeywordSearchActionItem& actionItem() const
    {
        return actionItem_;
    }

private:
    void parseOptions();

    /// @internal
    /// @brief Performs a full text search.
    /// @param[out] searchResult Search result from SMIA, including hit documents
    ///                       and raw text.
    bool doSearch(KeywordSearchResult& searchResult);


    /// @internal
    /// @brief Only gets raw text from SIA.
    /// @param getActionItem Action item to specify ID list.
    /// @param[out] rawTextResult Raw text.
    bool doGet(
        const GetDocumentsByIdsActionItem& getActionItem,
        const KeywordSearchResult& miaResult,
        RawTextResultFromMIA& rawTextResult
    );

    void filterDocIdList(const KeywordSearchResult& origin, const std::vector<sf1r::docid_t>& id_list, KeywordSearchResult& new_result);

    void renderDocuments(const RawTextResultFromMIA& rawTextResult);
    void renderDocuments(
        const KeywordSearchResult& siaResult
    );
    void renderMiningResult(const KeywordSearchResult& miaResult);
    void renderRangeResult(const KeywordSearchResult& searchResult);
    void renderCountResult(const KeywordSearchResult& searchResult);
    void renderRefinedQuery();

    bool validateSearchResult(const KeywordSearchResult& siaResult);
//     bool validateMiningResult(const KeywordSearchResult& miaResult);
    bool validateTextList(
        const std::vector<std::vector<PropertyValue::PropertyValueStrType> >& textList,
        std::size_t row,
        std::size_t column
    );

    void addAclFilters();

    /**
     * preprocess the search request @c actionItem_,
     * also update @p searchResult if necessary.
     */
    void preprocess(KeywordSearchResult& searchResult);

    bool checkSuffixMatchParam(std::string& message);

private:
    ::izenelib::driver::Request& request_;
    ::izenelib::driver::Response& response_;
    IndexSearchService* indexSearchService_;
    MiningSearchService* miningSearchService_;
    const IndexBundleSchema& indexSchema_;
    const MiningSchema& miningSchema_;
    const ZambeziConfig& zambeziConfig_;

    KeywordSearchActionItem actionItem_;

    int TOP_K_NUM;

    DocumentsRenderer renderer_;
};

} // namespace sf1r

#endif // PROCESS_CONTROLLERS_DOCUMENTS_SEARCH_HANDLER_H
