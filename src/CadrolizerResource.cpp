/* CadrolizerResource.cpp -- Cadrolizer IoTivity Resource.
 *
 * Copyright (C) 2015 Artyom V. Poptsov <poptsov.artyom@gmail.com>
 *
 * This file is part of Cadrolizer.
 *
 * Cadrolizer is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Cadrolizer is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cadrolizer.  If not, see <http://www.gnu.org/licenses/>.
 */

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

/* CadrolizerResource.cpp ends here. */