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

	static inline const std::string unnamed_object = "unnamed_object";

	// `inline` means that there exists only one (static) copy of this std::map,
	// independently of the number of its occurrences in different translation units.
    // This obviates the need in a .cpp file.

	// Note: This currently doesn't work with Intel's compiler (icc 19.0.1).
	// Can be fixed by adding a .cpp file
	static inline std::map<std::string, NamedObject*> named_objects;

	std::string name;
	bool name_attached;

protected:

	explicit NamedObject(std::string const & name_ = "")
	: name(name_), name_attached(false){
		attach_name();
	}

	NamedObject(NamedObject const & other) = delete;
	void operator=(NamedObject const & other) = delete;

	NamedObject(NamedObject && other) : name(""), name_attached(false){
		switch_object(other);
	}
    void operator=(NamedObject && other){
    	if (name_attached){
			detach_name(); // clear current data
		}
		switch_object(other);
    }

private:
	void switch_object(NamedObject & other){
		name = other.name;
		name_attached = other.name_attached;

		if (name_attached){
			other.name_attached = false;
	    	assert_with_message(0u != named_objects.count(name),
								"NamedObject: attached object missing from map");
	    	named_objects[name] = this;
		}
	}

	void remove_from_map(){
		if (name_attached){
			size_t const num_erased = named_objects.erase(name);
			assert_with_message(num_erased > 0,
								"NamedObject: object missing from map");
		}
	}

public:

	void attach_name(std::string const & new_name = ""){
		assert_with_message(false == name_attached,
							"NamedObject: object already attached");
		if (!new_name.empty()) {
			name = new_name;
		}
		if (name.empty()) {
			return;
		}
		name_attached = true;
    	assert_with_message(0u == named_objects.count(name),
							"NamedObject: name already used");
    	named_objects[name] = this;
	}

	void detach_name(){
		remove_from_map();
		name_attached = false;
	}

	virtual ~NamedObject(){
        detach_name();
    }
	
	void set_name(std::string const & new_name){
		remove_from_map();
		name = new_name;
		if (name_attached){
			named_objects[new_name] = this;
		}
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

	std::string const & get_name(){
		if (name != ""){
			return name;
		}
		return unnamed_object;
	}

    // Extensions to header here.
	
    // For example:
    // virtual void write_hdf5(H5::Group & h5group, std::string const & name_) const = 0;
	// void write_hdf5(H5::Group & h5group) const {
	// 	assert(name != "");
	// 	write_hdf5(h5group, name);
	// }
};

#endif /* NAMED_OBJECT_BASE_HPP_ */
