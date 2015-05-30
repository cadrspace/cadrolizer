#include <functional>
#include <cstdlib>

#include "OCPlatform.h"
#include "OCApi.h"

#include "CadrolizerResource.hpp"

using namespace OC;
using namespace std;
namespace PH = std::placeholders;

OCStackResult CadrolizerResource::handleGet(shared_ptr<OCResourceRequest> pRequest) {
	auto pResponse = make_shared<OCResourceResponse>();

	pResponse->setRequestHandle(pRequest->getRequestHandle());
	pResponse->setResourceHandle(pRequest->getResourceHandle());
	pResponse->setResourceRepresentation(this->get());
	pResponse->setErrorCode(200);
	pResponse->setResponseResult(OC_EH_OK);

	return OCPlatform::sendResponse(pResponse);
}

OCEntityHandlerResult CadrolizerResource::entityHandler(shared_ptr<OCResourceRequest> request) {
	OCEntityHandlerResult ehResult = OC_EH_ERROR;
	int requestFlag = request->getRequestHandlerFlag();
	if (requestFlag & RequestHandlerFlag::RequestFlag) {
		if (handleGet(request) == OC_STACK_OK)
			ehResult = OC_EH_OK;
	}
	return ehResult;
}
