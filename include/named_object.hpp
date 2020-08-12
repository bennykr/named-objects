/*
 * NamedObject.hpp
 *
 *  Created on: Aug 11, 2020
 *      Author: benny
 */

#ifndef NAMED_OBJECT_BASE_HPP_
#define NAMED_OBJECT_BASE_HPP_

#include <string>
#include <map>
#include <cassert>

#define assert_with_message(exp, msg) assert(((void)msg, exp))

// This class allows the user to retrieve an attached object by a string name.
// The retrieved pointer can be used directly (requires extension of this header),
// or cast to the derived type (assuming it is known).
class NamedObject{

private:

    // `inline` means that there exists only one (static) copy of this std::map,
    // independently of the number of its occurrences in different translation units.
    // This obviates the need in a .cpp file.
    // Note: This currently doesn't work with Intel's compiler (icc 19.0.1).
    // Can be fixed by adding a .cpp file
    static inline std::map<std::string, NamedObject*> named_objects;

    std::string name;

protected:

    explicit NamedObject(std::string const & name_) : name(name_){
        attach_name();
    }

    NamedObject(NamedObject const & other) = delete;
    void operator=(NamedObject const & other) = delete;

    NamedObject(NamedObject && other) : name() {
        switch_object(other);
    }
    void operator=(NamedObject && other){
        detach_name(); // clear current data
        switch_object(other);
    }

private:

    void attach_name(){
        assert_with_message(!name.empty(), "NamedObject: name must not be empty");

        auto const [iter, success] = named_objects.try_emplace(name, this);
        (void) iter; // avoid unused variable warning
        assert_with_message(success, "NamedObject: name already used");
    }

    void switch_object(NamedObject & other){
        assert_with_message(!other.name.empty(),
                            "NamedObject: moved object already detached");

        auto iter = named_objects.find(other.name);
        assert_with_message(named_objects.end() != iter,
                            "NamedObject: moved object missing from map");

        name = other.name;
        iter->second = this;
        other.name.clear();
    }

    void detach_name(){
        if (!name.empty()){
            size_t const num_erased = named_objects.erase(name);
            assert_with_message(num_erased > 0,
                                "NamedObject: trying to detache an object missing from map");
            name.clear();
        }
    }

public:

    virtual ~NamedObject(){
        detach_name();
    }
    
    static NamedObject * find(std::string const & name_){
        auto const iter = named_objects.find(name_);
        if (named_objects.end() == iter){
            return nullptr;
        }
        return iter->second;
    }

    static NamedObject & get(std::string const & name_){
        auto ptr = find(name_);
        assert_with_message(ptr != nullptr, "NamedObject: name missing from map");
        return *ptr;
    }

    void reset_name(std::string const & new_name){
        detach_name();
        name = new_name;
        attach_name();
    }

    std::string const & get_name(){
        return name;
    }

    // Extensions to header here.
    
    // For example:
    // virtual void write_hdf5(H5::Group & h5group, std::string const & name_) const = 0;
    // void write_hdf5(H5::Group & h5group) const {
    //     write_hdf5(h5group, name);
    // }
};

#endif /* NAMED_OBJECT_BASE_HPP_ */
