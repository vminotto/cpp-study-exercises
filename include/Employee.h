#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include <cstdint> 
#include <list>
#include <memory> 
#include <set>

namespace ex{

	using std::shared_ptr;

	/*8.2 Imagine you have a call center with three levels of employees: respondent,
	manager, and director. An incoming telephone call must be first allocated to a
	respondent who is free. If the respondent can't handle the call, he or she must
	escalate the call to a manager. If the manager is not free or not able to handle it,
	then the call should be escalated to a director. Design the classes and data structures
	for this problem. Implement a method dispatchCall() which assigns a
	call to the first available employee.
	*/
	class PhoneCall{
		enum class Status : std::uint8_t {
			Unpicked,
			Ongoing,
			Finished,
		};
	};

	class Employee{
	public:

		enum class Type : std::uint16_t{
			Respondent,
			Manager,
			Director
		};

		Employee(std::string personName, long id) : personName(personName), id(id) {}
		~Employee() = default;

		virtual void assignPhoneCall(PhoneCall *call){/*assign phone call, set the currentCall member, and make its status to 'Ongoing'
													  if a currentCall already exists, do throw error, because this method shuld not have
													  been called (a prior check to isFreeToPickPhone() should have been made.*/}
		virtual void finisheCurrentCall(){/*if currentCall exists, finish it, and set its status to 'Finished'*/ }

		virtual Type getType(){ return myType; }

		bool isFreeToPickPhone(){ return !currentCall; }

	protected:
		std::string personName;
		long id;
		Type myType;
		PhoneCall *currentCall = nullptr;

		//C++11 Allows in-declaration initializations
		double minimumWage = 0;
	};
	class Respondent : public Employee{

	public:
		Respondent(std::string personName, long id) : Employee(personName, id){
			minimumWage = 1000;
			myType = Employee::Type::Respondent;
		}
		~Respondent();
	};
	class Manager : public Employee{
	public:
		Manager(std::string personName, long id) : Employee(personName, id){
			minimumWage = 3000;
			myType = Employee::Type::Manager;
		}
		~Manager();
	};
	/*Here we inherit Director from Manager so that Director has all privilleges that a Manager
	has (in the	business point of view), besides some extra ones.*/
	class Director : public Manager{
	public:
		Director(std::string personName, long id) : Manager(personName, id){
			minimumWage = 6000;
			myType = Employee::Type::Director;
		}
		~Director();
	};

	class EmployeesRegister{
	public:
		static EmployeesRegister &getInstance(){
			static EmployeesRegister instance;
			return instance;
		}

		std::list<Employee*> *getEmployeesList(Employee::Type type){
			switch (type){
			case Employee::Type::Respondent:	return &respondents; break;
			case Employee::Type::Manager:		return &managers;	break;
			case Employee::Type::Director:		return &directors;	break;
			default: return nullptr;
			}
		}

	private:

		/*We assume that the company may have multiple managers and multiple directors as well,
		given the exercise did not constraint it. These need to of type 'Employee*' so that we 
		may leverage from polymorphism. In a production code, a smart pointer would be a better
		option.*/
		std::list<Employee*>	respondents; 
		std::list<Employee*>	   managers;
		std::list<Employee*>	  directors;

		EmployeesRegister(){
			/*Load employees' information from, e.g., a database*/
		}
		~EmployeesRegister(){}

		EmployeesRegister(EmployeesRegister const&);              // We do not implement these so that if tried to use, a compiler error is thrown.
		void operator=(EmployeesRegister const&);
	};

	/*This is a service class used to manager incoming phone calls.*/
	class CallManager {
	public:
	
		static CallManager &getInstance(){
			static CallManager instance;
			return instance;
		}

		/*Callback automatically called when a new phone call arrives. The funciton who calls
		this callback (which we won't implement) should pass a list of employee types that 
		may receive this call, in pryority order.*/
		void dispatchCall(PhoneCall *call, std::list<Employee::Type> priority){
			
			EmployeesRegister &empsReg = EmployeesRegister::getInstance();

			/*Here we must ensure all duplicates in 'priority' are removed, without
			changing the ordering of the list. A O(N^2)-time O(1)-size solution 
			would be a good option since 'priority' would usually be small list*/
			/* --- TODO ---*/

			for (auto type : priority){
				const std::list<Employee*> *empList = empsReg.getEmployeesList(type);
				if (empList){
					for (auto employee : *empList){
						if (employee->isFreeToPickPhone()){
							dispatchCall(call, employee);
							return;
						}
					}
				}
			}

			/*If this section is reached, the phone call could not be dispatched to anyone
			in the desired priority list. Therefore, we throw an exception.*/		
			throw std::string("Error in 'CallManager::manageNewCall(...)': phone call cannot be dispatched."); //In real code, an exception-like class would be thrown
		}

		

	private:
		/*Dispatches the call to the emplyee's branch line*/
		void dispatchCall(PhoneCall *call, Employee *employee){
			employee->assignPhoneCall(call);
			/*...*/
			employee->finisheCurrentCall();
		}

		CallManager(){}
		~CallManager(){}
		CallManager(EmployeesRegister const&);              // We do not implement these so that if tried to use, a compiler error is thrown.
		void operator=(CallManager const&);
	};

};

#endif