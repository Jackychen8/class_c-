#pragma once
#include "MiniCppUnit.hpp"
#include "rle-algo.hpp"
#include <iostream>
#include <iomanip>

// Helper function to compare two buffers
bool buffersAreSame(const char* expected, const char* actual, size_t size)
{
	bool retVal = true;

	if (actual == nullptr)
	{
		return false;
	}
	
	for (size_t i = 0; i < size; i++)
	{
		if (expected[i] != actual[i])
		{
			retVal = false;
			std::cerr << std::endl
				<< "Expected 0x" << std::hex << static_cast<unsigned>(expected[i])
				<< " at index " << std::dec << i << " but saw 0x"
				<< std::hex << static_cast<unsigned>(actual[i]) << std::endl;
			break;
		}
	}
	
	return retVal;
}


// IMPORTANT NOTE --
// The tests all use "sizeof" to find the size of the input
// buffer. However, remember that this only works because the
// character arrays are statically allocated.
// If you just have a pointer to a C-style string, "sizeof"
// will return the size of the pointer type (either 4 or 8).
class CompressionTests : public TestFixture<CompressionTests>
{
public:
	TEST_FIXTURE_DESCRIBE(CompressionTests, "Testing Compression...")
	{
		TEST_CASE_DESCRIBE(testBasicPositiveRuns, "Basic positive runs test");
		TEST_CASE_DESCRIBE(testBasicNegativeRuns, "Basic negative runs test");
		TEST_CASE_DESCRIBE(testPositiveNegativeMix, "Mixture of positive and negative runs test");
		TEST_CASE_DESCRIBE(testLongPositive, "Long positive run");
		TEST_CASE_DESCRIBE(testLongNegative, "Long negative run");
        //TEST_CASE_DESCRIBE(testNew, "New test");
	}
	
	void testBasicPositiveRuns()
	{
		char test[] = "aaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbc"
			"ccdddaaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbcccddd";
		char expected[] = "\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d";
		
		RLE<char> r;
		r.Compress(test, sizeof(test) - 1);
		ASSERT_TEST_MESSAGE(buffersAreSame(expected, r.m_Data, sizeof(expected) - 1),
							"Buffer mismatch (see above for details)");
	}
	
	void testBasicNegativeRuns()
	{
		char test[] = "abcdefghijklmnopqrstuvwxyz";
		char expected[] = "\xe6" "abcdefghijklmnopqrstuvwxyz";
		
		RLE<char> r;
		r.Compress(test, sizeof(test) - 1);
		ASSERT_TEST_MESSAGE(buffersAreSame(expected, r.m_Data, sizeof(expected) - 1),
							"Buffer mismatch (see above for details)");
	}
	
	void testPositiveNegativeMix()
	{
		char test[] = "abcddddefghhhhijkllllmnoppppqrsttttuvwxxxxy"
			"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz";
		char expected[] = "\xfd" "abc" "\x04" "d" "\xfd" "efg" "\x04" "h"
			"\xfd" "ijk" "\x04" "l" "\xfd" "mno" "\x04" "p"
			"\xfd" "qrs" "\x04" "t" "\xfd" "uvw" "\x04" "x"
			"\xff" "y" "\x30" "z";
		
		RLE<char> r;
		r.Compress(test, sizeof(test) - 1);
		ASSERT_TEST_MESSAGE(buffersAreSame(expected, r.m_Data, sizeof(expected) - 1),
							"Buffer mismatch (see above for details)");
	}
	
	void testLongPositive()
	{
		char test[] =
		"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
		char expected[] = "\x7f" "a" "\x7f" "a" "\x2e" "a";
		
		RLE<char> r;
		r.Compress(test, sizeof(test) - 1);
		ASSERT_TEST_MESSAGE(buffersAreSame(expected, r.m_Data, sizeof(expected) - 1),
							"Buffer mismatch (see above for details)");
	}
	
	void testLongNegative()
	{
		char test[] =
		"abcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyz";
		char expected[] = "\x81" "abcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvw"
		"\x81" "xyz"
		"abcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrst"
		"\xfa" "uvwxyz";
		
		RLE<char> r;
		r.Compress(test, sizeof(test) - 1);
		ASSERT_TEST_MESSAGE(buffersAreSame(expected, r.m_Data, sizeof(expected) - 1),
							"Buffer mismatch (see above for details)");
	}
    
    void testNew()
    {
        char test[] =
        "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
        "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
        "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
        "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
        "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
        "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
        "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
        "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
        "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
        "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
        "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
        "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"
        "\xff\xff\xff"
        "\x00\x00";
        char expected[] = "\x7b" "\xff"
        "\x02" "\x00";
        
        RLE<char> r;
        r.Compress(test, sizeof(test) - 1);
        ASSERT_TEST_MESSAGE(buffersAreSame(expected, r.m_Data, sizeof(expected) - 1),
                            "Buffer mismatch (see above for details)");
    }
};

class DecompressionTests : public TestFixture<DecompressionTests>
{
public:
	TEST_FIXTURE_DESCRIBE(DecompressionTests, "Testing Decompression...")
	{
		TEST_CASE_DESCRIBE(testBasicPositiveRuns, "Basic positive run test");
		TEST_CASE_DESCRIBE(testMultiplePositive, "Test multiple positive runs");
		TEST_CASE_DESCRIBE(testPositiveNegativeMix, "Test mixture of positive and negative runs");
	}
	
	void testBasicPositiveRuns()
	{
		char test[] = "\x28" "x";
		char expected[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
		
		RLE<char> r;
		r.Decompress(test, sizeof(test) - 1, sizeof(expected) - 1);
		ASSERT_TEST_MESSAGE(buffersAreSame(expected, r.m_Data, sizeof(expected) - 1),
							"Buffer mismatch (see above for details)");
	}
	
	void testMultiplePositive()
	{
		char test[] = "\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d";
		char expected[] = "aaabbbcccddd";
		
		RLE<char> r;
		r.Decompress(test, sizeof(test) - 1, sizeof(expected) - 1);
		ASSERT_TEST_MESSAGE(buffersAreSame(expected, r.m_Data, sizeof(expected) - 1),
							"Buffer mismatch (see above for details)");
	}
	
	void testPositiveNegativeMix()
	{
		char test[] = "\xfb" "abcde" "\x5" "a" "\x4" "b";
		char expected[] = "abcdeaaaaabbbb";
		
		RLE<char> r;
		r.Decompress(test, sizeof(test) - 1, sizeof(expected) - 1);
		ASSERT_TEST_MESSAGE(buffersAreSame(expected, r.m_Data, sizeof(expected) - 1),
							"Buffer mismatch (see above for details)");
	}
};

REGISTER_FIXTURE(CompressionTests);
REGISTER_FIXTURE(DecompressionTests);
