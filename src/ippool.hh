#ifndef _H_IPPOOL_IPPOOL
#define _H_IPPOOL_IPPOOL

#include <node_object_wrap.h>

#define IPPOOL_BIT_1(data, i) (data)[(i)/8] |= (1 << (7 - (i) % 8));

class Ippool : public node::ObjectWrap {
	public:
		static void Init();
		static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& args);
		
	private:
		template <int L>
		struct Range {
			union {
				uint8_t addr[L];
				uint32_t addr32[0];
				uint64_t addr64[0];
			};
			union {
				uint8_t mask[L];
				uint32_t mask32[0];
				uint64_t mask64[0];
			};
			int maskLen;
		};
		
		enum RangeComp {
			RC_DIFFERENT,
			RC_SAME,
			RC_1_INTO_2,
			RC_2_INTO_1,
		};
		
		typedef Range<4>  RangeV4;
		typedef Range<16> RangeV6;
		
		union GenericRange {
			Range<4>  v4;
			Range<16> v6;
		};
		
		static void add(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void addv4(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void addv6(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void search(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void searchv4(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void searchv6(const v8::FunctionCallbackInfo<v8::Value>& args);
		static void dump(const v8::FunctionCallbackInfo<v8::Value>& args);
		
		static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
		static v8::Persistent<v8::Function> constructor;
		
		static std::string IpToString(const RangeV4 &range);
		static std::string IpToString(const RangeV6 &range);
		static int GetAddr(v8::Handle<v8::String> str, GenericRange &gr, int version = 0);
		static int GetAddr(const std::string &str, GenericRange &gr, int version = 0);
		bool StoreIpAddr(v8::Handle<v8::String> str, int version = 0);
		bool StoreIpAddr(const std::string &str, int version = 0);
		static int ParseMask(const std::string &maskStr, int version);
		bool searchv4(const RangeV4 &range);
		bool searchv6(const RangeV6 &range);
		
		std::vector<RangeV4> v4pool;
		std::vector<RangeV6> v6pool;
		
		template <int L>
		static RangeComp rangeCompare(const Range<L> &r1, const Range<L> &r2) {
			bool eq1 = true;
			bool eq2 = true;
			
			#define x(r, i) (((uint32_t*)r)[i])
			for(int i = 0, l = L / 4 ; i < l && x(r1.mask, i) && x(r2.mask, i) ; i++) {
				eq1 = eq1 && ((x(r1.addr, i) & x(r2.mask, i)) == x(r2.addr, i));
				eq2 = eq2 && ((x(r2.addr, i) & x(r1.mask, i)) == x(r1.addr, i));
			}
			#undef x
			
			if(eq1 && eq2) {
				if(r1.maskLen > r2.maskLen)
					return(Ippool::RC_1_INTO_2);
				else if(r1.maskLen < r2.maskLen)
					return(Ippool::RC_2_INTO_1);
				return(Ippool::RC_SAME);
			}
			else if(eq1) {
				return(Ippool::RC_1_INTO_2);
			}
			else if(eq2) {
				return(Ippool::RC_2_INTO_1);
			}
			
			return(Ippool::RC_DIFFERENT);
		}

};

#endif
