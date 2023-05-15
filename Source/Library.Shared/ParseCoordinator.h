#pragma once

#include "RTTI.h"
#include "Scope.h"
#include "ParseHelper.h"
#include "vector.h"
#include <cstdint>
#include <cstddef>
#include <initializer_list>
#include <cstdlib>
#include <cassert>
#include <memory>
#include <stdexcept>
#include <utility>
#include <cstring>
#include <string>
#include <functional>
#include <iostream>
#include <fstream>
#include <iosfwd>
#include <sstream>
#include <json/json.h>

namespace FieaGameEngine
{
    class ParseCoordinator final : public RTTI
    {
        RTTI_DECLARATIONS(ParseCoordinator, FieaGameEngine::RTTI);

    private:
        std::shared_ptr<Vector<StackFrame>> _contextStack = nullptr;
        Vector<std::shared_ptr<ParseHelper>> _helperList;
        std::shared_ptr<Wrapper_Base> _wrapperPtr;

    public:
        /// <summary>
        /// Ctor for Coordinator, takes in shared_ptr to a wrapperObj. this is provided by the user.
        /// </summary>
        /// <param name="wrapperOBJ"></param>
        ParseCoordinator(std::shared_ptr<Wrapper_Base> wrapperOBJ);
        /// <summary>
        /// Adds shared_ptr of a user instantiated helper to the _HelperList vector.
        /// </summary>
        /// <param name="helper"></param>
        void AddHelper(std::shared_ptr<ParseHelper> helper);
        /// <summary>
        /// Removes specified helper from the _HelperList.
        /// </summary>
        /// <param name="helper"></param>
        void RemoveHelper(std::shared_ptr<ParseHelper> helper);
        /// <summary>
        /// Given a filename, deserializes it into the provided wrapper. Iterates through the list of Helpers
        /// till a Helper returns true on its ability to deserialize the given object (StartHandlerCall). EndHandler is called against this Helper.
        /// if no helper is capable of deserialization, throws a runtime error.
        /// </summary>
        /// <param name="data"></param>
        void DeserializeObjectFromFile(const std::string& filename);
        /// <summary>
        /// Deserializes from a given string.
        /// </summary>
        /// <param name="data"></param>
        void DeserializeObject(const std::string& data);
        /// <summary>
        /// Main function that is fed by filename and string Deserialize methods.
        /// </summary>
        /// <param name="inputStream"></param>
        void DeserializeObject(std::istream& inputStream);

        /// <summary>
        /// function to do descent. Takes in a jsonObject and other parameters. requires the Stack frame that contains information of current scope and current key
        /// to be set before being called.
        /// </summary>
        /// <param name="jsonObj">Json Object to be deserialized</param>
        /// <param name="isArray"></param>
        /// <param name="index"></param>
        void ParseValue(const Json::Value& jsonObj, bool isArray = false, std::size_t index = 0);
        /// <summary>
        /// function called by ParseValue that takes the jsonValue, and checks whether it is an object, array or primitive.
        /// </summary>
        /// <param name="key"></param>
        /// <param name="value"></param>
        /// <param name="isArray"></param>
        /// <param name="index"></param>
        void ParseKeyValuePair(const std::string& key, const Json::Value& value, bool isArray, std::size_t index);

        void Cleanup();
    };

    struct StackFrame
    {
        Scope* _currentScope = nullptr;
        std::string _currentKey;
        int _nTableHandles = 0;
        std::string _currentType{ "NotSet" };
        std::string _className{ "NotSet" };

        StackFrame(Scope* scopePtr, std::string key) :
            _currentScope{ scopePtr }, _currentKey{ key }
        {
        }

    };

    class Wrapper_Base : public RTTI
    {
        RTTI_DECLARATIONS(Wrapper_Base, FieaGameEngine::RTTI);

        std::size_t _depth = 0; //should this be in the Parse coordinator? or base wrapper?
        std::weak_ptr<ParseCoordinator> _coordinatorPtr;

    public:
        //virtual Wrapper* Create();

        /// <summary>
        /// Sets the weak_ptr ref to the Coordinator this object has been passed to. To be called by the Coordinator on initialization.
        /// </summary>
        /// <param name="coordinator"></param>
        void SetParseCoordinator(std::shared_ptr<ParseCoordinator> coordinator);
        /// <summary>
        /// Returns reference to the associated Coordinator
        /// </summary>
        /// <returns></returns>
        std::weak_ptr<ParseCoordinator> GetParseCoordinator();
        void IncrementDepth();
        void DecrementDepth();
        std::size_t CurrentDepth();

    };


}