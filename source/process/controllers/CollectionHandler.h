#ifndef PROCESS_CONTROLLERS_COLLECTIONHANDLER_H
#define PROCESS_CONTROLLERS_COLLECTIONHANDLER_H
/**
 * @file process/controllers/CollectionHandler.h
 * @author Ian Yang
 * @date Created <2011-01-25 17:30:12>
 */

#include <bundles/index/IndexBundleConfiguration.h>
#include <bundles/index/IndexSearchService.h>
#include <bundles/index/IndexTaskService.h>
#include <bundles/mining/MiningBundleConfiguration.h>
#include <bundles/mining/MiningSearchService.h>
#include <bundles/recommend/RecommendSchema.h>

#include <util/driver/Request.h>
#include <util/driver/Response.h>
#include <util/driver/Value.h>

namespace sf1r
{

class RecommendTaskService;
class RecommendSearchService;

/**
 * @brief CollectionHandler
 *
 * Each collection has its corresponding collectionHandler
 *
 */
class CollectionHandler
{
public:
    CollectionHandler(const std::string& collection);

    ~CollectionHandler();
public:
    //////////////////////////////////////////
    //    Handlers
    //////////////////////////////////////////
    void search(::izenelib::driver::Request& request, ::izenelib::driver::Response& response);
    void get(::izenelib::driver::Request& request, ::izenelib::driver::Response& response);
    void similar_to(::izenelib::driver::Request& request, ::izenelib::driver::Response& response);
    void duplicate_with(::izenelib::driver::Request& request, ::izenelib::driver::Response& response);
    void similar_to_image(::izenelib::driver::Request& request, ::izenelib::driver::Response& response);
    void create(const ::izenelib::driver::Value& document);
    void update(const ::izenelib::driver::Value& document);
    void destroy(const ::izenelib::driver::Value& document);


    //////////////////////////////////////////
    //    Helpers
    //////////////////////////////////////////
    void registerService(IndexSearchService* service)
    {
        indexSearchService_ = service;
    }

    void registerService(IndexTaskService* service)
    {
        indexTaskService_ = service;
    }

    void registerService(MiningSearchService* service)
    {
        miningSearchService_ = service;
    }

    void registerService(RecommendTaskService* service)
    {
        recommendTaskService_ = service;
    }

    void registerService(RecommendSearchService* service)
    {
        recommendSearchService_ = service;
    }

    void setBundleSchema(IndexBundleSchema& schema)
    {
        indexSchema_ = schema;
    }

    void setBundleSchema(MiningSchema& schema)
    {
	miningSchema_ = schema;
    }

    void setBundleSchema(RecommendSchema& schema)
    {
	recommendSchema_ = schema;
    }
public:
    std::string collection_;

    IndexSearchService* indexSearchService_;

    IndexTaskService* indexTaskService_;

    MiningSearchService* miningSearchService_;

    RecommendTaskService* recommendTaskService_;

    RecommendSearchService* recommendSearchService_;

    IndexBundleSchema indexSchema_;

    MiningSchema miningSchema_;

    RecommendSchema recommendSchema_;
};

} // namespace sf1r

#endif // PROCESS_CONTROLLERS_COLLECTIONHANDLER_H
