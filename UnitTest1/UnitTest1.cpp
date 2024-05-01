#include "pch.h"
#include "CppUnitTest.h"
#include "../PR_11.5/PR_11.5.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestAddRecordToFile)
		{
			vector<Record> records;
			Record newRecord;


			appendToFile("testRecords.bin", newRecord);

			ifstream file("testRecords.bin", ios::binary);
			Assert::IsTrue(file.good());
			file.close();
		}
	};
}
