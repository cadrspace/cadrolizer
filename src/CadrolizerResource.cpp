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
#include "CadrolizerException.hpp"

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

OCStackResult CadrolizerResource::handlePut(shared_ptr<OCResourceRequest> pRequest)
{
        auto pResponse = make_shared<OCResourceResponse>();
        OCRepresentation rep = pRequest->getResourceRepresentation();

        try {
                this->put(rep);
                pResponse->setErrorCode(200);
                pResponse->setResponseResult(OC_EH_OK);
        } catch (CadrolizerException &e) {
                syslog(LOG_WARNING, e.what());
                pResponse->setErrorCode(400);
                pResponse->setResponseResult(OC_EH_ERROR);
        }

        pResponse->setResourceRepresentation(this->get());

        return OCPlatform::sendResponse(pResponse);
}

OCEntityHandlerResult CadrolizerResource::entityHandler(shared_ptr<OCResourceRequest> request) {
        OCEntityHandlerResult ehResult = OC_EH_ERROR;
        int requestFlag = request->getRequestHandlerFlag();
        string requestType = request->getRequestType();
        if (requestFlag & RequestHandlerFlag::RequestFlag) {
                if (requestType == "GET") {
                        if (handleGet(request) == OC_STACK_OK)
                                ehResult = OC_EH_OK;
                } else if (requestType == "PUT") {
                        if (handlePut(request) == OC_STACK_OK)
                                ehResult = OC_EH_OK;
                }
        }
        return ehResult;
}

static const string STATE_SHUTDOWN = "shutdown";
static const string STATE_REBOOT   = "reboot";

/**
 * Handle state change.  Throw 'CadrolizerException' on an error.
 *
 * @param state A new state to handle.
 */
void CadrolizerResource::handleState(string &state)
{
        if (state == STATE_SHUTDOWN) {
                if (m_isShutdownAllowed) {
                        m_state = state;
                        OS::shutdown();
                } else {
                        throw CadrolizerException(
                                "shutdown is not allowed");
                }
        } else if (state == STATE_REBOOT) {
                if (m_isShutdownAllowed) {
                        m_state = state;
                        OS::reboot();
                } else {
                        throw CadrolizerException(
                                "shutdown is not allowed");
                }
        } else {
                ostringstream os;
                os << "Unknown state: " << state << endl;
                throw CadrolizerException(os.str().c_str());
        }
}

/**
 * Handle a PUT request.
 *
 * @param rep A resource representation.
 */
void CadrolizerResource::put(OCRepresentation& rep)
{
        string state;
        if (rep.getValue("state", state)) {
                ostringstream os;
                os << "State: " << state << endl;
                syslog(LOG_INFO, os.str().c_str());
                handleState(state);
        } else {
                throw CadrolizerException(
                        "No state provided.");
        }
}

/* CadrolizerResource.cpp ends here. */
