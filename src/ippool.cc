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
	Isolate* isolate = Isolate::GetCurrent();
	// Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
	tpl->SetClassName(String::NewFromUtf8(isolate, "ippool"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
#define SETFUNC(_name_) \
	NODE_SET_PROTOTYPE_METHOD(tpl, #_name_, _name_);
	SETFUNC(add)
	SETFUNC(addv4)
	SETFUNC(addv6)
	SETFUNC(search)
	SETFUNC(searchv4)
	SETFUNC(searchv6)
	SETFUNC(dump)
	
	constructor.Reset(isolate, tpl->GetFunction());
}

void Ippool::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	
	if(args.IsConstructCall()) {
		// Invoked as constructor: `new Ippool(...)`
		Ippool* obj = new Ippool();
		obj->Wrap(args.Holder());
		args.GetReturnValue().Set(args.Holder());
	}
	else {
		// Invoked as plain function `Ippool(...)`, turn into construct call.
		const int argc = 0;
		Local<Value> argv[argc] = { };
		Local<Function> cons = Local<Function>::New(isolate, constructor);
		args.GetReturnValue().Set(cons->NewInstance(argc, argv));
	}
}

void Ippool::NewInstance(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	
	const unsigned argc = 0;
	Handle<Value> argv[argc] = { };
	Local<Function> cons = Local<Function>::New(isolate, constructor);
	Local<Object> instance = cons->NewInstance(argc, argv);
	
	args.GetReturnValue().Set(instance);
}

void Ippool::add(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	
	Ippool *obj = ObjectWrap::Unwrap<Ippool>(args.Holder());
	
	if(args.Length() == 0) {
		args.GetReturnValue().Set(args.Holder());
		return;
	}
	
	for(int i = 0 ; i < args.Length() ; i++) {
		if(args[i]->IsString()) {
			obj->StoreIpAddr(args[i]->ToString());
		}
	}
	
	args.GetReturnValue().Set(args.Holder());
	return;
}

void Ippool::addv4(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	
	Ippool *obj = ObjectWrap::Unwrap<Ippool>(args.Holder());
	
	if(args.Length() == 0) {
		args.GetReturnValue().Set(args.Holder());
		return;
	}
	
	for(int i = 0 ; i < args.Length() ; i++) {
		if(args[i]->IsString()) {
			obj->StoreIpAddr(args[i]->ToString(), 4);
		}
	}
	
	args.GetReturnValue().Set(args.Holder());
	return;
}

void Ippool::addv6(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	
	Ippool *obj = ObjectWrap::Unwrap<Ippool>(args.Holder());
	
	if(args.Length() == 0) {
		args.GetReturnValue().Set(args.Holder());
		return;
	}
	
	for(int i = 0 ; i < args.Length() ; i++) {
		if(args[i]->IsString()) {
			obj->StoreIpAddr(args[i]->ToString(), 6);
		}
	}
	
	args.GetReturnValue().Set(args.Holder());
	return;
}

void Ippool::search(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	
	Ippool *obj = ObjectWrap::Unwrap<Ippool>(args.Holder());
	Ippool::GenericRange gr;
	int version;
	
	if(args.Length() != 1) {
		isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Invalid argument count!")));
		return;
	}
	
	if(!args[0]->IsString()) {
		isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Invalid argument type!")));
		return;
	}
	
	version = Ippool::GetAddr(args[0]->ToString(), gr);
	
	if(version == 4)
		args.GetReturnValue().Set(Boolean::New(isolate, obj->searchv4(gr.v4)));
	else if(version == 6)
		args.GetReturnValue().Set(Boolean::New(isolate, obj->searchv6(gr.v6)));
	else
		args.GetReturnValue().Set(Boolean::New(isolate, false));
}

void Ippool::searchv4(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	
	Ippool *obj = ObjectWrap::Unwrap<Ippool>(args.Holder());
	Ippool::GenericRange gr;
	int version;
	
	if(args.Length() != 1) {
		isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Invalid argument count!")));
		return;
	}
	
	if(!args[0]->IsString()) {
		isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Invalid argument type!")));
		return;
	}
	
	version = Ippool::GetAddr(args[0]->ToString(), gr, 4);
	
	if(version == 4)
		args.GetReturnValue().Set(Boolean::New(isolate, obj->searchv4(gr.v4)));
	else
		args.GetReturnValue().Set(Boolean::New(isolate, false));
}

void Ippool::searchv6(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	
	Ippool *obj = ObjectWrap::Unwrap<Ippool>(args.Holder());
	Ippool::GenericRange gr;
	int version;
	
	if(args.Length() != 1) {
		isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Invalid argument count!")));
		return;
	}
	
	if(!args[0]->IsString()) {
		isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Invalid argument type!")));
		return;
	}
	
	version = Ippool::GetAddr(args[0]->ToString(), gr, 6);
	
	if(version == 6)
		args.GetReturnValue().Set(Boolean::New(isolate, obj->searchv6(gr.v6)));
	else
		args.GetReturnValue().Set(Boolean::New(isolate, false));
}

void Ippool::dump(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	
	Ippool *obj = ObjectWrap::Unwrap<Ippool>(args.Holder());
	Local<Object> ret = Object::New(isolate);
	Local<Array> arr;
	std::string strBuffer;
	
	
	arr = Array::New(isolate, obj->v4pool.size());
	for(int i = 0, l = obj->v4pool.size() ; i < l ; i++) {
		strBuffer = Ippool::IpToString(obj->v4pool[i]);
		arr->Set(i, String::NewFromUtf8(isolate, strBuffer.c_str()));
	}
	
	ret->Set(String::NewFromUtf8(isolate, "v4"), arr);
	
	arr = Array::New(isolate, obj->v6pool.size());
	for(int i = 0, l = obj->v6pool.size() ; i < l ; i++) {
		strBuffer = Ippool::IpToString(obj->v6pool[i]);
		arr->Set(i, String::NewFromUtf8(isolate, strBuffer.c_str()));
	}
	
	ret->Set(String::NewFromUtf8(isolate, "v6"), arr);
	
	args.GetReturnValue().Set(ret);
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
	String::Utf8Value ip(str);
	return(Ippool::GetAddr1(*ip, gr, version));
}

int Ippool::GetAddr1(const std::string &str, Ippool::GenericRange &gr, int version) {
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
		if(uv_inet_pton(AF_INET, sub.c_str(), &address_buffer) != 0)
			return(0);
	}
	else if(version == 6) {
		if(uv_inet_pton(AF_INET6, sub.c_str(), &address_buffer) != 0)
			return(0);
	}
	else {
		if(uv_inet_pton(AF_INET, sub.c_str(), &address_buffer) == 0)
			version = 4;
		else if(uv_inet_pton(AF_INET6, sub.c_str(), &address_buffer) == 0)
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
		
		gr.v4.addr32[0] &= gr.v4.mask32[0];
		
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
		
		gr.v6.addr64[0] &= gr.v6.mask64[0];
		gr.v6.addr64[1] &= gr.v6.mask64[1];
		
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
	String::Utf8Value ip(str);
	return(this->StoreIpAddr1(*ip, version));
}

bool Ippool::StoreIpAddr1(const std::string &str, int version) {
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
			if(uv_inet_pton(AF_INET, maskStr.c_str(), &address_buffer) == 0) {
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
			if(uv_inet_pton(AF_INET6, maskStr.c_str(), &address_buffer) == 0) {
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
