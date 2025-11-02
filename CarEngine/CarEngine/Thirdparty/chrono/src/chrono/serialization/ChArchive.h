// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2014 projectchrono.org
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================

#ifndef CHARCHIVE_H
#define CHARCHIVE_H

#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <typeinfo>
#include <unordered_set>
#include <memory>
#include <algorithm>

#include "chrono/core/ChApiCE.h"
#include "chrono/core/ChStream.h"
#include "chrono/core/ChClassFactory.h"
#include "chrono/core/ChTemplateExpressions.h"

namespace chrono {

/// @addtogroup chrono_serialization
/// @{

///
/// This is a base class for name-value pairs
///

template<class T>
class  ChNameValue {
  public:
        ChNameValue(const char* mname, const T& mvalue, char mflags = 0) : 
            _name(mname), 
            _value((T*)(& mvalue)),
            _flags((char)mflags) {}

        ChNameValue(const ChNameValue<T>& other){
            _name  = other._name;
            _value = other._value;
            _flags = other._flags;
        }
        
        ~ChNameValue() {};

        const char * name() const {
            return this->_name;
        }

        char& flags() {
            return this->_flags;
        }

        T & value() const {
            return *(this->_value);
        }

        const T & const_value() const {
            return *(this->_value);
        }

  protected:
        const char* _name;
        T* _value;
        char _flags;
};

// Flag to mark a ChNameValue for a C++ object serialized by value but that
// that can be later referenced by pointer too.
static const char NVP_TRACK_OBJECT = (1 << 0);

template<class T>
ChNameValue< T > make_ChNameValue(const char * auto_name, const T & t, const char * custom_name, char flags = 0){
    const char* mname = auto_name;
    if (custom_name)
        mname = custom_name;
    return ChNameValue< T >(mname, t, flags);
}
template<class T>
ChNameValue< T > make_ChNameValue(const char * auto_name, const T & t, char flags = 0){
    const char* mname = auto_name;
    return ChNameValue< T >(mname, t, flags);
}

/// Macros to create ChNameValue objects easier

// utilities to avoid the ##__VA_ARGS__ that is not yet portable: 
#define FIRST(...) FIRST_HELPER(__VA_ARGS__, throwaway)
#define FIRST_HELPER(first, ...) first
#define REST(...) REST_HELPER(NUM(__VA_ARGS__), __VA_ARGS__)
#define REST_HELPER(qty, ...) REST_HELPER2(qty, __VA_ARGS__)
#define REST_HELPER2(qty, ...) REST_HELPER_##qty(__VA_ARGS__)
#define REST_HELPER_ONE(first)
#define REST_HELPER_TWOORMORE(first, ...) , __VA_ARGS__
#define NUM(...)  SELECT_10TH(__VA_ARGS__, TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE, ONE, throwaway)
#define SELECT_10TH(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, ...) a10

#define STRINGIFY0(v) #v
#define STRINGIFY(v) STRINGIFY0(v)

/// Use this macro to mark a value, ex  
///    myarchive << CHNVP (myvalue, "mnemonic name")
/// or, not providing the mnemonic name, the name will be get from the C name of the variable:
///    myarchive << CHNVP (myvalue)

#define CHNVP(...) \
    make_ChNameValue("" STRINGIFY(FIRST(__VA_ARGS__)) "", FIRST(__VA_ARGS__) REST(__VA_ARGS__) )

///////////////////////////////

/// Class that handle C++ values of generic type using type erasure and functors.
/// For example used to call the ArchiveOUT function for unrelated classes that
/// implemented them. This helps stripping out the templating, to make ChArchiveOut
/// easier and equippable with virtual functions.

class ChValue {
public:
    virtual ~ChValue() {};
    
    virtual ChValue* new_clone() = 0;

    // 
    // Type helpers
    // 
        /// Get registered name in class factory. If type is not previously registered, 
        /// returns a "" string.
        /// This is platform-independent.
    virtual std::string& GetClassRegisteredName() =0;

        /// Get class version, if class version is registered, otherwise defaults 0
    virtual int GetClassRegisteredVersion() =0;

        /// Get platform-dependent typeid name of referenced data
    virtual const char* GetTypeidName() =0;

        /// Get platform-dependent typeid of referenced data
    virtual const std::type_info* GetTypeid() =0;

        /// Tell if it is a null pointer    
    virtual bool IsNull()=0;

        /// Tell if the underlying original type is polymorphic
    virtual bool  IsPolymorphic() = 0;
        /// Tell if the underlying original type is an array
    virtual bool  IsArray() = 0;
        /// Tell if the underlying original type is a class
    virtual bool  IsClass() = 0;
        /// Tell if the underlying original type is a pointer
    virtual bool  IsPointer() = 0;

        /// Access the data by a raw pointer, given as static_cast
    virtual void* GetRawPtr() =0;

        /// Get name of property
    const char * name() const {
            return this->_name.c_str();
        }

        /// Get flags of property
    char& flags() {
            return this->_flags;
        }

    //    
    // Casting:
    // 

        /// Use this to do safe dynamic cast. 
        /// This uses the cast/throw trick for dynamic cast after type-erasure; note that this 
        /// has performance penalty respect to usual dynamic_cast<>, which is not possible here.
        /// Note: can only upcast, but no downcast (i.e. U must be higher the TClass used
        /// when instantiating ChValueSpecific); otherwise use a later dynamic_cast<>.
    template <typename U>
    U* PointerUpCast() {
        try { this->thrower(); }
        catch (U* ptr) { return static_cast<U*>(ptr); }
        catch (...) {}
        return 0;
    }

protected:
    virtual void thrower() const = 0; 

    //const char* _name;
    std::string _name;
    char _flags;

};

template <class TClass> 
class ChValueSpecific : public ChValue
{
private:
      TClass* _ptr_to_val;   // pointer to value
public:

      // constructor 
      ChValueSpecific(TClass& mvalp, const char* mname, char flags) { 
          _ptr_to_val = (TClass*)(& mvalp);
          _name = mname;
          _flags = flags;
      };
      // constructor 
      ChValueSpecific(ChNameValue< TClass > mVal) {
          _ptr_to_val = mVal.value();
          _name = mVal.name();
          _flags = mVal.flags();
      };

      virtual ChValue* new_clone() {
          return new ChValueSpecific<TClass>(*_ptr_to_val, _name.c_str(), _flags);
      }

      virtual std::string& GetClassRegisteredName() {
          static std::string nostring("");
          if (!_ptr_to_val) {
              return nostring;
          }
          try {
              return ChClassFactory::GetClassTagName(typeid(*_ptr_to_val));
          }catch (const ChException &) {
              return nostring;
          }
        }

      virtual int GetClassRegisteredVersion() {
          return chrono::class_factory::ChClassVersion<TClass>::version;
        }

      virtual const std::type_info* GetTypeid() {
          return &typeid(TClass);
      }

      virtual const char* GetTypeidName() {
          return GetTypeid()->name();
      }

      virtual bool IsNull()
        { return (_ptr_to_val==0);};   


      virtual void* GetRawPtr() 
        { return static_cast<void*>(_ptr_to_val); };

      virtual bool IsPolymorphic() 
        { return std::is_polymorphic<TClass>::value; };
      
      virtual bool IsArray()
        { return std::is_array<TClass>::value; };

      virtual bool IsClass()
        { return std::is_class<TClass>::value; };

      virtual bool IsPointer() 
        { return std::is_pointer<TClass>::value; };

private:
        virtual void thrower() const {
             throw static_cast<TClass*>(_ptr_to_val); 
          }
};











/// Class for mapping enums to ChNameValue pairs that contain a 'readable' ascii string
/// of the selected enum. This could be used when streaming from/to human readable formats
/// such as JSON or XML or ascii dumps.

class ChEnumMapperBase {
public:
    ChEnumMapperBase ()  {}

    virtual int  GetValueAsInt() = 0;
    virtual void SetValueAsInt(const int mval) = 0;

    virtual std::string GetValueAsString() = 0;
    virtual bool SetValueAsString(const std::string& mname) = 0;
};

template <class Te>
class ChEnumNamePair {
public:
    ChEnumNamePair(const char* mname, Te menumid) : name(mname), enumid(menumid) {}

    std::string name;
    Te enumid;
};

template <class Te>
class ChEnumMapper : public ChEnumMapperBase {
  public:
    ChEnumMapper() : value_ptr(0) {
        enummap = std::shared_ptr<std::vector<ChEnumNamePair<Te> > >(new std::vector<ChEnumNamePair<Te> >);
    }

    ChEnumMapper(std::shared_ptr<std::vector<ChEnumNamePair<Te> > > mmap) : value_ptr(0), enummap(mmap) {}

    virtual ~ChEnumMapper() {}

    void AddMapping(const char* name, Te enumid) {
        ChEnumNamePair<Te> mpair (name, enumid);
        enummap->push_back(mpair);
    }
    void AddMapping(const char* autoname, Te enumid, const char* custom_name) {
        const char* name = autoname;
        if (custom_name)
            name = custom_name;
        ChEnumNamePair<Te> mpair (name, enumid);
        enummap->push_back(mpair);
    }

    Te& Value() {return *value_ptr;}

    virtual int  GetValueAsInt() override { 
        return static_cast<int>(*value_ptr);
    };
    virtual void SetValueAsInt(const int mval) override {
        *value_ptr = static_cast<Te>(mval);
    };

    virtual std::string GetValueAsString() override {
        for (int i = 0; i < enummap->size(); ++i)
        {
            if(enummap->at(i).enumid == *value_ptr)
                return enummap->at(i).name;
        }
        // not found, just string as number:
        char buffer [10];
        sprintf(buffer, "%d", GetValueAsInt());
        return std::string(buffer);
    };

    virtual bool SetValueAsString(const std::string& mname) override {
        for (int i = 0; i < enummap->size(); ++i) {
            if (enummap->at(i).name == mname) {
                *value_ptr = enummap->at(i).enumid;
                return true;
            }
        }
        // try to find from integer:
        int numb;
        std::istringstream mstream(mname);
        mstream >> numb;
        if (mstream.fail())
            return false;

        // Set value from number
        SetValueAsInt(numb);
        return true;
    };

    Te* value_ptr;

 protected:
        
    std::shared_ptr< std::vector< ChEnumNamePair<Te> > > enummap;
};

/// Three macros to simplify the use of enum mapper.
/// Use them always in sequence, with nothing else in between. 
/// Possibly, use them just after you defined an enum.
/// After this, you have a class called "MyEnum_mapper", inherited
/// from ChEnumMapper, and you can use it for converting enums from/to strings.
/// Example:
///
/// enum eChMyenum {
///           ATHLETIC = 0,
///           SKINNY = 3,
///           FAT
///   };
///
///  CH_ENUM_MAPPER_BEGIN(MyEnum);
///   CH_ENUM_VAL(ATHLETIC);
///   CH_ENUM_VAL(SKINNY);
///   CH_ENUM_VAL(FAT, "fatty");  // overrides the "FAT" mapped string with "fatty"
///  CH_ENUM_MAPPER_END(MyEnum);
/// 

#define CH_ENUM_MAPPER_BEGIN(__enum_type) \
            class __enum_type##_mapper : public ChEnumMapper< __enum_type > { \
            public: \
                __enum_type##_mapper() { 

#define CH_ENUM_VAL(...) \
        this->AddMapping("" STRINGIFY(FIRST(__VA_ARGS__)) "", FIRST(__VA_ARGS__) REST(__VA_ARGS__) );

#define CH_ENUM_MAPPER_END(__enum_type) \
            }; \
        ChEnumMapper< __enum_type > operator() ( __enum_type & mval) { \
            ChEnumMapper< __enum_type > res(this->enummap); \
            res.value_ptr = &mval; \
            return res; \
        } };

//
// Wrapper class to ease the archival of std::pair
//

template<class T, class Tv>
class _wrap_pair {
public:
    _wrap_pair(std::pair<T, Tv>& apair) {
        _wpair = &apair;
    }

private:
    std::pair<T, Tv>* _wpair;
};

/// @} chrono_serialization

}  // end namespace chrono

#endif
