/*
 * Copyright (c) 2010-2014 BinarySEC SAS
 * Ippool binding for nodejs [http://www.binarysec.com]
 * 
 * This file is part of Gate.js.
 * 
 * Gate.js is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "module.hh"

using namespace v8;

Persistent<Function> Ippool::constructor;

void Ippool::Init() {
	// Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("ippool"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	
#define SETFUNC(_name_) \
	tpl->PrototypeTemplate()->Set( \
		String::NewSymbol(#_name_), \
		FunctionTemplate::New(_name_)->GetFunction() \
	);
	SETFUNC(add)
	SETFUNC(addv4)
	SETFUNC(addv6)
	SETFUNC(search)
	SETFUNC(searchv4)
	SETFUNC(searchv6)
	SETFUNC(dump)
	
	constructor = Persistent<Function>::New(tpl->GetFunction());
}

v8::Handle<v8::Value> Ippool::New(const v8::Arguments& args) {
	HandleScope scope;
	
	if(args.IsConstructCall()) {
		// Invoked as constructor: `new Ippool(...)`
		Ippool* obj;
		obj = new Ippool();
		obj->Wrap(args.This());
		
		return(args.This());
	}
	else {
		// Invoked as plain function `Ippool(...)`, turn into construct call.
		const unsigned argc = 0;
		Handle<Value> *argv = NULL;
		return(scope.Close(constructor->NewInstance(argc, argv)));
	}
}

Handle<Value> Ippool::NewInstance(const Arguments& args) {
	HandleScope scope;
	
	const unsigned argc = 0;
	Handle<Value> *argv = NULL;
	Local<Object> instance = constructor->NewInstance(argc, argv);
	
	return(scope.Close(instance));
}

Handle<Value> Ippool::add(const Arguments& args) {
	HandleScope scope;
	Ippool *obj = node::ObjectWrap::Unwrap<Ippool>(args.This());
	
	if(args.Length() == 0)
		return(scope.Close(args.This()));
	
	for(int i = 0 ; i < args.Length() ; i++) {
		if(args[i]->IsString()) {
			obj->StoreIpAddr(args[i]->ToString());
		}
	}
	
	return(scope.Close(args.This()));
}

Handle<Value> Ippool::addv4(const Arguments& args) {
	HandleScope scope;
	Ippool *obj = node::ObjectWrap::Unwrap<Ippool>(args.This());
	
	if(args.Length() == 0)
		return(scope.Close(args.This()));
	
	for(int i = 0 ; i < args.Length() ; i++) {
		if(args[i]->IsString()) {
			obj->StoreIpAddr(args[i]->ToString(), 4);
		}
	}
	
	return(scope.Close(args.This()));
}

Handle<Value> Ippool::addv6(const Arguments& args) {
	HandleScope scope;
	Ippool *obj = node::ObjectWrap::Unwrap<Ippool>(args.This());
	
	if(args.Length() == 0)
		return(scope.Close(args.This()));
	
	for(int i = 0 ; i < args.Length() ; i++) {
		if(args[i]->IsString()) {
			obj->StoreIpAddr(args[i]->ToString(), 6);
		}
	}
	
	return(scope.Close(args.This()));
}

Handle<Value> Ippool::search(const Arguments& args) {
	HandleScope scope;
	Ippool *obj = node::ObjectWrap::Unwrap<Ippool>(args.This());
	Ippool::GenericRange gr;
	int version;
	
	if(args.Length() != 1) {
		return(ThrowException(Exception::Error(String::New("Invalid argument count!"))));
	}
	
	if(!args[0]->IsString()) {
		return(ThrowException(Exception::Error(String::New("Invalid argument type!"))));
	}
	
	version = Ippool::GetAddr(args[0]->ToString(), gr);
	
	if(version == 4)
		return(scope.Close(Boolean::New(obj->searchv4(gr.v4))));
	else if(version == 6)
		return(scope.Close(Boolean::New(obj->searchv6(gr.v6))));
	else
		return(scope.Close(Boolean::New(false)));
}

Handle<Value> Ippool::searchv4(const Arguments& args) {
	HandleScope scope;
	Ippool *obj = node::ObjectWrap::Unwrap<Ippool>(args.This());
	Ippool::GenericRange gr;
	int version;
	
	if(args.Length() != 1) {
		return(ThrowException(Exception::Error(String::New("Invalid argument count!"))));
	}
	
	if(!args[0]->IsString()) {
		return(ThrowException(Exception::Error(String::New("Invalid argument type!"))));
	}
	
	version = Ippool::GetAddr(args[0]->ToString(), gr, 4);
	
	if(version == 4)
		return(scope.Close(Boolean::New(obj->searchv4(gr.v4))));
	else
		return(scope.Close(Boolean::New(false)));
}

Handle<Value> Ippool::searchv6(const Arguments& args) {
	HandleScope scope;
	Ippool *obj = node::ObjectWrap::Unwrap<Ippool>(args.This());
	Ippool::GenericRange gr;
	int version;
	
	if(args.Length() != 1) {
		return(ThrowException(Exception::Error(String::New("Invalid argument count!"))));
	}
	
	if(!args[0]->IsString()) {
		return(ThrowException(Exception::Error(String::New("Invalid argument type!"))));
	}
	
	version = Ippool::GetAddr(args[0]->ToString(), gr, 6);
	
	if(version == 6)
		return(scope.Close(Boolean::New(obj->searchv6(gr.v6))));
	else
		return(scope.Close(Boolean::New(false)));
}

Handle<Value> Ippool::dump(const Arguments& args) {
	HandleScope scope;
	Ippool *obj = node::ObjectWrap::Unwrap<Ippool>(args.This());
	Local<Object> ret = Object::New();
	Local<Array> arr;
	std::string strBuffer;
	
	
	arr = Array::New(obj->v4pool.size());
	for(int i = 0, l = obj->v4pool.size() ; i < l ; i++) {
		strBuffer = Ippool::IpToString(obj->v4pool[i]);
		arr->Set(i, String::New(strBuffer.c_str()));
	}
	
	ret->Set(String::New("v4"), arr);
	
	arr = Array::New(obj->v6pool.size());
	for(int i = 0, l = obj->v6pool.size() ; i < l ; i++) {
		strBuffer = Ippool::IpToString(obj->v6pool[i]);
		arr->Set(i, String::New(strBuffer.c_str()));
	}
	
	ret->Set(String::New("v6"), arr);
	
	return(scope.Close(ret));
}

std::string Ippool::IpToString(const Ippool::RangeV4 &range) {
	char buffer[1024];
	std::string ret;
	
	uv_inet_ntop(AF_INET, range.addr, buffer, sizeof(buffer));
	
	ret = buffer;
	
	snprintf(buffer, sizeof(buffer), "/%d", range.maskLen);
	ret += buffer;
	
	return(ret);
}

std::string Ippool::IpToString(const Ippool::RangeV6 &range) {
	char buffer[1024];
	std::string ret;
	
	uv_inet_ntop(AF_INET6, range.addr, buffer, sizeof(buffer));
	
	ret = buffer;
	
	snprintf(buffer, sizeof(buffer), "/%d", range.maskLen);
	ret += buffer;
	
	return(ret);
}

int Ippool::GetAddr(Handle<String> str, Ippool::GenericRange &gr, int version) {
	String::AsciiValue ip(str);
	return(Ippool::GetAddr(*ip, gr, version));
}

int Ippool::GetAddr(const std::string &str, Ippool::GenericRange &gr, int version) {
	char address_buffer[sizeof(struct in6_addr)];
	std::string sub;
	size_t pos;
	int mask;
	
	pos = str.find('/');
	
	if(pos == std::string::npos)
		sub = str;
	else
		sub = str.substr(0, pos);
	
	if(version == 4) {
		if(uv_inet_pton(AF_INET, sub.c_str(), &address_buffer).code != UV_OK)
			return(0);
	}
	else if(version == 6) {
		if(uv_inet_pton(AF_INET6, sub.c_str(), &address_buffer).code != UV_OK)
			return(0);
	}
	else {
		if(uv_inet_pton(AF_INET, sub.c_str(), &address_buffer).code == UV_OK)
			version = 4;
		else if(uv_inet_pton(AF_INET6, sub.c_str(), &address_buffer).code == UV_OK)
			version = 6;
		else
			return(0);
	}
	
	if(pos == std::string::npos) {
		if(version == 4)
			mask = 32;
		else if(version == 6)
			mask = 128;
	}
	else {
		sub = str.substr(pos + 1);
		mask = Ippool::ParseMask(sub, version);
	}
	
	memset(&gr, 0, sizeof(gr));
	if(version == 4) {
		memcpy(gr.v4.addr, address_buffer, sizeof(gr.v4.addr));
		
		for(int i = 0 ; i < mask ; i++) {
			if(i < mask)
				IPPOOL_BIT_1(gr.v4.mask, i)
		}
		
		((uint32_t*)gr.v4.addr)[0] &= ((uint32_t*)gr.v4.mask)[0];
		
		gr.v4.maskLen = mask;
		//printf("Addr: %02X", gr.v4.addr[0]);
		//for(size_t i = 1 ; i < sizeof(gr.v4.addr) ; i++)
			//printf(".%02X", gr.v4.addr[i]);
		//printf("\nMask: %02X", gr.v4.mask[0]);
		//for(size_t i = 1 ; i < sizeof(gr.v4.mask) ; i++)
			//printf(".%02X", gr.v4.mask[i]);
		//printf("\nMaskLen: %d\n", gr.v4.maskLen);
	}
	else if(version == 6) {
		memcpy(gr.v6.addr, address_buffer, sizeof(gr.v6.addr));
		
		for(int i = 0 ; i < mask ; i++) {
			if(i < mask)
				IPPOOL_BIT_1(gr.v6.mask, i)
		}
		
		((uint64_t*)gr.v6.addr)[0] &= ((uint64_t*)gr.v6.mask)[0];
		((uint64_t*)gr.v6.addr)[1] &= ((uint64_t*)gr.v6.mask)[1];
		
		gr.v6.maskLen = mask;
		//printf("Addr: %02X", gr.v6.addr[0]);
		//for(size_t i = 1 ; i < sizeof(gr.v6.addr) ; i++)
			//printf(".%02X", gr.v6.addr[i]);
		//printf("\nMask: %02X", gr.v6.mask[0]);
		//for(size_t i = 1 ; i < sizeof(gr.v6.mask) ; i++)
			//printf(".%02X", gr.v6.mask[i]);
		//printf("\nMaskLen: %d\n", gr.v6.maskLen);
	}
	
	return(version);
}

bool Ippool::StoreIpAddr(Handle<String> str, int version) {
	String::AsciiValue ip(str);
	return(this->StoreIpAddr(*ip, version));
}

bool Ippool::StoreIpAddr(const std::string &str, int version) {
	Ippool::GenericRange gr;
	
	version = Ippool::GetAddr(str, gr, version);
	
	if(version == 4) {
		std::vector<RangeV4>::iterator it;
		std::vector<RangeV4>::iterator end;
		Ippool::RangeComp rc;
		
		it = this->v4pool.begin();
		end = this->v4pool.end();
		
		for(; it != end ; ) {
			rc = Ippool::rangeCompare<4>(*it, gr.v4);
			if(rc == Ippool::RC_1_INTO_2) {
				it = this->v4pool.erase(it);
				end = this->v4pool.end();
			}
			else if(rc == Ippool::RC_2_INTO_1)
				return(false);
			else if(rc == Ippool::RC_SAME)
				return(false);
			else
				it++;
		}
		
		this->v4pool.push_back(gr.v4);
	}
	else if(version == 6) {
		std::vector<RangeV6>::iterator it;
		std::vector<RangeV6>::iterator end;
		Ippool::RangeComp rc;
		
		it = this->v6pool.begin();
		end = this->v6pool.end();
		
		for(; it != end ; ) {
			rc = Ippool::rangeCompare<16>(*it, gr.v6);
			if(rc == Ippool::RC_1_INTO_2) {
				it = this->v6pool.erase(it);
				end = this->v6pool.end();
			}
			else if(rc == Ippool::RC_2_INTO_1)
				return(false);
			else if(rc == Ippool::RC_SAME)
				return(false);
			else
				it++;
		}
		
		this->v6pool.push_back(gr.v6);
	}
	else
		return(false);
	
	return(true);
}

int Ippool::ParseMask(const std::string &maskStr, int version) {
	char address_buffer[sizeof(struct in6_addr)];
	int mask = -1;
	
	if(version == 4) {
		if(maskStr.find('.') != std::string::npos) {
			if(uv_inet_pton(AF_INET, maskStr.c_str(), &address_buffer).code == UV_OK) {
				for(int i = 0 ; i < 32 ; i++) {
					if((address_buffer[i/8] & (1 << (i%8))) == 0)
						return(i+1);
				}
			}
		}
		
		mask = atoi(maskStr.c_str());
		if(mask < 0 || mask > 32)
			mask = 32;
	}
	else if(version == 6) {
		if(maskStr.find(':') != std::string::npos) {
			if(uv_inet_pton(AF_INET6, maskStr.c_str(), &address_buffer).code == UV_OK) {
				for(int i = 0 ; i < 128 ; i++) {
					if((address_buffer[i/8] & (1 << (i%8))) == 0)
						return(i+1);
				}
			}
		}
		
		mask = atoi(maskStr.c_str());
		if(mask < 0 || mask > 128)
			mask = 128;
	}
	
	return(mask);
}

bool Ippool::searchv4(const RangeV4 &range) {
	std::vector<RangeV4>::iterator it;
	std::vector<RangeV4>::iterator end;
	Ippool::RangeComp rc;
	
	it = this->v4pool.begin();
	end = this->v4pool.end();
	
	for(; it != end ; it++) {
		if(range.maskLen < it->maskLen)
			continue;
		
		rc = Ippool::rangeCompare<4>(range, *it);
		if(rc == Ippool::RC_SAME || rc == Ippool::RC_1_INTO_2)
			return(true);
	}
	
	return(false);
}

bool Ippool::searchv6(const RangeV6 &range) {
	std::vector<RangeV6>::iterator it;
	std::vector<RangeV6>::iterator end;
	Ippool::RangeComp rc;
	
	it = this->v6pool.begin();
	end = this->v6pool.end();
	
	for(; it != end ; it++) {
		if(range.maskLen < it->maskLen)
			continue;
		
		rc = Ippool::rangeCompare<16>(range, *it);
		if(rc == Ippool::RC_SAME || rc == Ippool::RC_1_INTO_2)
			return(true);
	}
	
	return(false);
}
