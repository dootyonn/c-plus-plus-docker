#include <gtest/gtest.h>
#include <example.pb.h>

#include <print>
#include <iostream>
#include <format>
#include <algorithm>

namespace Test {

    void ListPeoples(const tutorial::AddressBook& address_book) {
        for (int i = 0; i < address_book.people_size(); i++) {
            const tutorial::Person& person = address_book.people(i);
        
            std::cout << "Person ID: " << person.id() << std::endl;
            std::cout << "  Name: " << person.name() << std::endl;
            if (person.has_email()) {
                std::cout << "  E-mail address: " << person.email() << std::endl;
            }
        
            for (int j = 0; j < person.phones_size(); j++) {
                const tutorial::Person::PhoneNumber& phone_number = person.phones(j);
        
                switch (phone_number.type()) {
                case tutorial::Person::PHONE_TYPE_MOBILE:
                    std::cout << "  Mobile phone #: ";
                    break;
                case tutorial::Person::PHONE_TYPE_HOME:
                    std::cout << "  Home phone #: ";
                    break;
                case tutorial::Person::PHONE_TYPE_WORK:
                    std::cout << "  Work phone #: ";
                    break;
                }
                std::cout << phone_number.number() << std::endl;
            }
        }
    }

    TEST(ProtoTest, test_proto) {
        tutorial::AddressBook address_book;

        {
            auto* person = address_book.add_people();
            person->set_id(1);
            person->set_name("My Name");
            person->set_email("my.name@myname.gov");

            auto* phone = person->add_phones();
            phone->set_number("+1-123-456-7890");
            phone->set_type(tutorial::Person_PhoneType_PHONE_TYPE_MOBILE);
        }
        
        
        std::vector<u_char> buffer;

        {
            std::size_t size = address_book.ByteSizeLong();
            buffer.resize(size);
            address_book.SerializeToArray(buffer.data(), buffer.size());

            std::print("Data: ");
            for (auto value : buffer) {
                std::print("0x{:02x} ", value);
            }
            std::println("");
        }

        {
            address_book.ParseFromArray(buffer.data(), buffer.size());

            ListPeoples(address_book);
        }


    }
}