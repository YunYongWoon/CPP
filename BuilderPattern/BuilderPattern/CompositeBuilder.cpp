#include <iostream>
#include <string>
#include <utility>
using namespace std;

class Person;
class PersonBuilder;
class PersonBuilderBase;

class Person {
	string street_address, post_code, city;	// 주소
	string company_name, position;			// 직장
	int incoming = 0;
	Person() {

	};
public:
	friend class PersonBuilder;
	friend class PersonAddressBuilder;
	friend class PersonJobBuilder;

	static PersonBuilder create();
};

class PersonBuilderBase {
protected:
	Person& person;
	explicit PersonBuilderBase(Person& person) : person{ person } {

	}

public:
	operator Person() {
		return move(person);
	}

	PersonAddressBuilder lives() const;

	PersonJobBuilder works() const;
};

class PersonBuilder : public PersonBuilderBase {
	Person p;
public:
	PersonBuilder() : PersonBuilderBase{ p } {
	};
};

class PersonAddressBuilder : public PersonBuilderBase {
public:
	explicit PersonAddressBuilder(Person& person) : PersonBuilderBase{ person } {

	}

	PersonAddressBuilder& at(string street_address) {
		person.street_address = street_address;
		return *this;
	}

	PersonAddressBuilder& with_postcode(string post_code) {
		person.post_code = post_code;
		return *this;
	}

	PersonAddressBuilder& in(string city) {
		person.city = city;
		return *this;
	}
};

class PersonJobBuilder : public PersonBuilderBase {
public:
	explicit PersonJobBuilder(Person& person) : PersonBuilderBase{ person } {

	}

	PersonJobBuilder& at(string company_name) {
		person.company_name = company_name;
		return *this;
	}

	PersonJobBuilder& as_a(string position) {
		person.position = position;
		return *this;
	}

	PersonJobBuilder& earning(int incoming) {
		person.incoming = incoming;
		return *this;
	}
};

PersonBuilder Person::create() {
	return PersonBuilder{};
}

PersonAddressBuilder PersonBuilderBase::lives() const {
	return PersonAddressBuilder{ person };
}

PersonJobBuilder PersonBuilderBase::works() const {
	return PersonJobBuilder{ person };
}

int main() {
	Person p = Person::create()
		.lives().at("123 London Road").with_postcode("SW1 1GB").in("London")
		.works().at("PragmaSoft").as_a("Consultant").earning(1000);
}