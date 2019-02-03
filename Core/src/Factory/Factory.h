#pragma once
#include <functional>
#include <map>

#include <IOManip/json.hpp>

template<class T>
class Factory
{
public:
	virtual std::unique_ptr<T> Build( const std::string& key, const nlohmann::json& params ) = 0;
};

