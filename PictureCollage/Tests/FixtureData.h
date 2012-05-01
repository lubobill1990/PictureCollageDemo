#pragma once

#include "common.h"
enum categery{outer=0x0002,inner=0x0004,bound=0x0008};
class FixtureData{
private: 
	categery m_FixtureType;
	std::vector<b2Fixture *> m_ContactFixtureList;
	b2Fixture* m_Fixture;
public:
	FixtureData(categery fixtureType){
		this->m_FixtureType=fixtureType;
	}
	categery GetFixtureType(){
		return m_FixtureType;
	}
	b2Fixture* GetFixture(){
		return this->m_Fixture;
	}
	void SetFixture(b2Fixture* fixture){
		this->m_Fixture=fixture;
	}	
	void AddContactedFixture(b2Fixture *fixture){
		FixtureData *fixData=(FixtureData*)fixture->GetUserData();
		m_ContactFixtureList.push_back(fixture);
	}
	void DeleteContactedFixture(b2Fixture *fixture)
	{
		std::vector<b2Fixture *>::iterator itr=remove(m_ContactFixtureList.begin(),m_ContactFixtureList.end(),fixture);
		if (itr!=m_ContactFixtureList.end())
		{
			m_ContactFixtureList.erase(itr);
		}
	}
	bool HaveContactedFixture(b2Fixture *fixture){
		for (uint i=0;i< m_ContactFixtureList.size();++i)
		{
			if (m_ContactFixtureList[i]==fixture)
			{
				return true;
			}
		}
		return false;
	}
	inline std::vector<b2Fixture *> GetContactedFixtureList()
	{
		return m_ContactFixtureList;
	}
	void PrintFixtureData(){
		std::string output="Fixture type: ";
		switch(this->m_FixtureType){
		case outer:
			output+="outer";
			break;
		case inner:
			output+="inner";
			break;
		default:
			output+="unknown";
			break;
		}
		output+=" | ";
		
		BodyData *bd=(BodyData*)this->m_Fixture->GetBody()->GetUserData();
		output+="Body id: ";
		std::cout<<output<<+bd->m_Id<<std::endl;
	}
	void PrintContactedFixture(){
		std::vector<b2Fixture*> list=this->GetContactedFixtureList();
		std::cout<<"------- Contacted Fixture -------"<<std::endl;
		if (list.size()==0)
		{
			return;
		}
		for (uint i=0;i<list.size();++i)
		{
			b2Fixture* tmpFixture=list[i];
			FixtureData *fd=(FixtureData*)tmpFixture->GetUserData();
			fd->PrintFixtureData();
		}
		std::cout<<"------- ----------------- -------"<<std::endl;
	}

};
