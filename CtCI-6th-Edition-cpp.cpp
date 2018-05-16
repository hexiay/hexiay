
/*===================================================*/
/*  ./Ch 1.Arrays And Strings/7.Rotate_matrix/7.rotate_matrix.cpp  */
/*
 * Cracking the coding interview edition 6
 * Problem 1.7 Rotate a matrix by 90' clockwise ( or anticlockwise).
 * Solution : I have done it two ways.
 * Approach 1: Take transpose of matrix and then reverse the rows for clockwise 90' rotation.
 * 			   Obviously if we reverse the columns we will get anticlockwise 90' rotation.
 * Approach 2: As mentioned in the book, rotating invididual elements layer by layer.
 * 			   I have solved it perform anticlockwise 90' rotation, it can be done similarly for clockwise rotatation.
 */
#include<iostream>

void helper_transpose(int **matrix, int N)
{
	for( int i = 0; i < N; ++i ) {
		for( int j = i+1; j < N; ++j ) {
			if ( i != j ) {
				std::swap(matrix[i][j], matrix[j][i]);
			}
		}
	}
}

void helper_reverse( int * row, int N ) {
	for ( int i = 0; i < N/2; ++i ) {
		std::swap(row[i], row[N-i-1]);
	}
}

void rotate1(int ** matrix, int N) {
	//transpose matrix
	helper_transpose(matrix, N);
	// reverse each row
	for ( int i = 0; i < N; ++i ) {
		helper_reverse(matrix[i], N);
	}
}


void rotate2( int ** matrix, int N ) {
	for( int i = 0; i < N/2; ++i ) {
		for( int j = i; j < N-i-1; ++j ) {
				int temp = matrix[i][j];
				matrix[i][j] = matrix[j][N-i-1];
				matrix[j][N-i-1] = matrix[N-i-1][N-j-1];
				matrix[N-i-1][N-j-1]= matrix[N-j-1][i];
				matrix[N-j-1][i] = temp;
		}
	}
}

void printMatrix( int ** matrix, int N) {
	for ( int i = 0; i < N; ++i ) {
		for( int j = 0; j < N; ++j ) {
			std::cout << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
}


int main() {
	int N;
	std::cout << "Enter N for NxN matrix:";
	std::cin >> N;
	int ** matrix = new int*[N];
	for ( int i = 0; i < N; ++i ) {
		matrix[i] = new int[N];
	}

	for ( int i = 0; i < N; ++i) {
		for ( int j = 0; j < N; ++j ) {
			std::cin >> matrix[i][j];
		}
	}

	std::cout << "Rotated matrix by 90 (clockwise):\n";
	rotate1(matrix, N);
	printMatrix(matrix, N);

	std::cout << "Rotated matrix again by 90(anticlockwise):\n";
	rotate2(matrix, N);
	printMatrix(matrix, N);
	return 0;
}
/*===================================================*/
/*  ./Ch 1.Arrays And Strings/5.One_Away/5-one-edit-away.cpp  */
/* 
 * Problem: There are three possible edits that can be performed on a string.
 * 1. Insert a char.
 * 2. Delete a char.
 * 3. Replace a char.
 *
 * Given two strings, determine if they are one or 0 edit away.
 *
 * Approach :
 * 1. Case when strings are of some length --> possible edit is replace.
 *    If there are more than one mismatch, return false
 *
 * 2. Case when One string is bigger than another
 *    Smaller string ------------> Bigger String
 *                     insert
 *                     delete
 *    smaller string <-----------  Bigger String
 *
 *    Idea is check if there are more than one mismatch discounting the already
 *    difference in the string. Therefore for first mismatch we do not move the pointer
 *    pointing to smaller string, and then expect it to match from next char of bigger
 *    string.
 */



#include <iostream>
#include <string>
#include <cmath>


bool oneEditAway( const std::string & str1, const std::string & str2 )
{
    if ( std::abs( int(str1.length()) - int(str2.length()))  > 1 ) {
        return false;
    }

    int len1 = str1.length();
    int len2 = str2.length();
    std::string smaller = len1 < len2 ? str1 : str2;
    std::string bigger =  len1 < len2 ? str2 : str1;
    
    unsigned int i = 0, j = 0; 
    bool mismatchDone = false;
    while ( i < smaller.length() && j < bigger.length() )
    {
        if ( smaller[i] != bigger[j] ) {
            if (mismatchDone) {
                return false;
            }
            mismatchDone = true;
            if ( len1 == len2 ) {
                ++i;   //case of replace
            }
        } else {
                ++i;   //move short pointer if its a match, dont move it in case of first mismatch
        }
        ++j;           //always move long string pointer.
    }
    return true;
}


void translate( bool result, const std::string str1, const std::string str2 )
{
    if (result == true ) {
        std::cout << str1 << " and " << str2 << " are one edit away\n";
    } else {
        std::cout << str1 << " and " << str2 << " are not one edit away\n";
    }
}

int main()
{
    translate ( oneEditAway("pale", "ple"), "pale", "ple" );
    translate ( oneEditAway("pales", "pale"), "pales", "pale" );
    translate ( oneEditAway("pale", "pales"), "pale", "pales" );
    translate ( oneEditAway("pale", "bale"), "pale", "bale" );
    translate ( oneEditAway("pale", "bake"), "pale", "bake" );
    return 0;

}
/*===================================================*/
/*  ./Ch 1.Arrays And Strings/6.String_Compression/6.string_compression.cpp  */
/*
 * Cracking the coding interview edition 6
 * Problem 1-6 Implement a method to perform basic string compression.
 * Example string aabcccccaaa should be compressed to a2b1c5a3,
 * however if compressed string is bigger than original string, return original string
 */

#include <iostream>
#include <string>


std::string compress(std::string str)
{
	size_t original_length = str.length();
	if (original_length < 2) {
		return str;
	}
	std::string out{""};
	int count = 1;
	for( size_t i = 1; i < original_length; ++i ) {
		if (str[i-1] == str[i]) {
			++count;
		} else {
			out += str[i-1];
			out += std::to_string(count);
			count = 1;
		}
		if (out.length() >= original_length) {
			return str;
		}
	}
	out += str[original_length-1];
	out += std::to_string(count);
	if (out.length() >= original_length) {
		return str;
	}
	return out;
}

int main()
{
	std::string str, out;
	std::cout << "Enter a string:\n";
	std::cin >> str;
	out = compress(str);
	if (str.compare(out)) {
		std::cout << str << " can be compressed to " << out << std::endl;
	} else {
		std::cout << str << " can not be compressed\n";
	}
	return 0;
}
/*===================================================*/
/*  ./Ch 1.Arrays And Strings/4.Palindrome_Permutation/4-pallindrome-permutations.cpp  */
/*
 * Cracking the coding interview edition 6
 * Given a string, write a function to check if it is a permutation of a pallindrome.
 *
 * Solution Philosophy:
 * For a string to be pallindrome, it should be able to spelled backward and forward the same.
 * Therefore the chars in string should fit one of the two possibilities:
 *  - Each char appear even number of times in the string ( even length string )
 *  - Each char should appear even number of times, except just one char ( odd length string )
 * 
 * We won't care about the case of the letter
 */

#include <iostream>


/*
 * Helper routine to return an frequency Table index
 *
 */

int getCharIndex( char c )
{
    int idx = -1;
    if ( c >= 'a' && c <= 'z' )
    {
        idx = c - 'a';
    }
    else if ( c >= 'A' && c <= 'Z' )
    {
        idx = c - 'A';
    }
    return idx;
}

/*
 * Function : countFrequency
 * Args     : input string, an array of int 
 * Return   : Void, array of int will populate each letter's frequency in string.
 */

void countFrequency( const std::string & str, int *frequency )
{
    int idx;
    for (const char & c : str)
    {
        idx = getCharIndex(c);
        if ( idx != -1 )
        {
            ++frequency[idx];
        }
    }
}


/*
 * Function : isPermutePallindrome 
 * Args     : input string
 * Return   : returns true if is possible that one of the permutations of input string can be a pallindrome.
 *            else return false
 */

bool isPermutationOfPallindrome1( const std::string & str )
{
    int frequency[ 26 ] = { 0 };
    countFrequency( str, frequency );

    /*
     * We will check here that letter frequencies are all even or all even except one odd.
     */
    bool oddAppeared = false;
    std::cout << std::endl;
    for ( int i = 0 ; i < 26; ++i ) {
        if ( frequency[i] % 2  && oddAppeared ) {
            return false;
        } else if ( frequency[i] % 2 && !oddAppeared ) {
            oddAppeared = true;
        }
    }
    return true;
}


/*
 * Approach 2:
 * Let us optimize above function instead of taking another pass let us do it
 * in one go, we will count odd chars as we go along, if we are left with 
 * more that 0 or 1, then the input string can't have pallindrome permutation
 */

bool isPermutationOfPallindrome2( const std::string & str )
{
    int oddCount = 0;
    int frequency[26] = { 0 };
    int idx = 0;
    for ( const char & c : str )
    {
        idx = getCharIndex(c);
        if ( idx != -1 )
        {
            ++frequency[idx];
            if ( frequency[idx] % 2 ) 
            {   
                ++oddCount;
            } else {
                --oddCount;
            }
        }
    }
    return (oddCount <= 1);
}

/*
 * Approach 3
 * let us represent each char with a bit in a bitvector
 * Each time a char appears in the string we toggle the
 * respective bit, if we are left with more than 1 bit
 * in the bit vector, the string can not have a pallidrome
 * permutation.
 *
 */

/*
 * helper function to toggle a bit in the integer
 */ 

int toggle( int bitVector, int index )
{
    if ( index < 0 )
        return bitVector;

    int mask = 1 << index;
    //if bit is not set
    if ( (bitVector & mask ) == 0 )
    {
        bitVector |= mask;
    } else {    //if bit is set
        bitVector &= ~mask;
    }
    return bitVector;
}

/*
 * Helper functiont to find if a single bit is set
 * i.e. if bitVector is a multiple of power of 2
 */

bool isExactlyOneBitSet( int bitVector )
{
    return ( (bitVector & (bitVector - 1)) == 0 );
}

/*
 * Third approach solution
 * toggle bit represent the respective char 
 * for each appearance in the string.
 */

bool isPermutationOfPallindrome3( const std::string & str )
{
    int bitVector = 0;
    int id = 0;
    for ( const char & c : str )
    {
        id = getCharIndex(c);
        bitVector = toggle (bitVector, id );
    }
    return ( bitVector == 0 || isExactlyOneBitSet(bitVector) );
}

int main()
{
    std::string str("Tact Coa");
    std::cout << "Does \"" << str << "\"  has a string whose permutation is a pallindrome? "
              << "( 1 for true, 0 for false ) : ";
    std::cout << "Approach 1:" << isPermutationOfPallindrome1( str ) << std::endl;
    std::cout << "Approach 2:" << isPermutationOfPallindrome2( str ) << std::endl;
    std::cout << "Approach 3:" << isPermutationOfPallindrome3( str ) << std::endl;


    std::string str1("A big Cat");
    std::cout << "Does \"" << str1 << "\" has a string whose permutation is a pallindrome? "
              << "( 1 for true, 0 for false ) : ";
    std::cout << "Approach 1:" << isPermutationOfPallindrome1( str1 ) << std::endl;
    std::cout << "Approach 2:" << isPermutationOfPallindrome2( str1 ) << std::endl;
    std::cout << "Approach 3:" << isPermutationOfPallindrome3( str1 ) << std::endl;


    std::string str2("Aba cbc");
    std::cout << "Does \"" << str2 << "\" has a string whose permutation is a pallindrome? "
              << "( 1 for true, 0 for false ) : ";
    std::cout << "Approach 1:" << isPermutationOfPallindrome1( str2 ) << std::endl;
    std::cout << "Approach 2:" << isPermutationOfPallindrome2( str2 ) << std::endl;
    std::cout << "Approach 3:" << isPermutationOfPallindrome3( str2 ) << std::endl;
    return 0;
}
/*===================================================*/
/*  ./Ch 1.Arrays And Strings/3.Palindrome Permutation/Palindrome Permutation.cpp  */
#include <string>
#include <vector>
#include <iostream>

using namespace std;

int getCharNumber(const char & c){
    int a = (int) 'a';
    int z = (int) 'z';
    int A = (int) 'A';
    int Z = (int) 'Z';
    int val = (int) c;
    if(a <= val && val <= z){
        return val - 'a';
    }
    else if(A <= val && val <= Z){
        return val - 'A';
    }
    return -1;
}


vector <int> buildCharFrequencyTable(string phrase){
    vector <int> table(getCharNumber('z') - getCharNumber('a') + 1, 0);
    for(char &c : phrase){
        int x = getCharNumber(c);
        if(x != -1){
            table[x]++;
        }
    }
    return table;
}


bool checkMaxOneOdd(vector<int> &table)
{
    bool foundOdd = false;
    for (auto count : table)
    {
        if (count % 2 == 1)
        {
            if (foundOdd)
            {
                return false;
            }
            foundOdd = true;
        }
    }
    return true;
}

bool isPermutationOfPalindrome(const string &phrase)
{
    vector<int> table = buildCharFrequencyTable(phrase);
    return checkMaxOneOdd(table);
}

int main(int argc, const char *argv[])
{
    string pali = "Rats live on no evil star";
    string isPermutation = isPermutationOfPalindrome(pali) ? "yes" : "no";
    cout << isPermutation << endl;
    return 0;
}
/*===================================================*/
/*  ./Ch 1.Arrays And Strings/2.Check Permutation/1.Check_Permutation.cpp  */
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <iostream>
using namespace std;
bool arePermutation(string str1,string str2)
{
    // Get lengths of both strings
    int n1 = str1.length();
    int n2 = str2.length();

    // If length of both strings is not same, then they
    // cannot be anagram
    if (n1 != n2)
      return false;

    // Sort both strings
    sort(str1.begin(), str1.end());
    sort(str2.begin(), str2.end());
    // Compare sorted strings
    for (int i = 0; i < n1;  i++)
       if (str1[i] != str2[i])
         return false;

    return true;
}

bool arePermutation_2(const string &str1, const string &str2) {
  if(str1.length() != str2.length()) 
    return false;
  int count[256]={0};
  for(int i = 0; i < str1.length(); i++) {
    int val = str1[i];
    count[val]++;
  }
  for(int i = 0; i < str2.length(); i++) {
    int val = str2[i];
    count[val]--;
    if(count[val]<0) 
      return false;
  }
  return true;
}
int main() {
// Test Method 1 - Using sort
    cout << "Method 1 - Using sort" << endl;
    string str1 = "testest";
    string str2 = "estxest";
    if(arePermutation(str1, str2))
      cout << str1 <<" and " << str2 << " are permutation of each other" << endl;
    else
      cout << str1 <<" and " << str2 << " are not permutation of each other" << endl;
    str1 = "hello";
    str2 = "oellh";
    if(arePermutation(str1, str2))
      cout << str1 <<" and " << str2 << " are permutation of each other" << endl;
    else
      cout << str1 <<" and " << str2 << " are not permutation of each other" << endl;

//Test Method 2 - Using character count
    cout << "Method 2 - Using character count" << endl;
    str1 = "testest";
    str2 = "estxest";
    if(arePermutation_2(str1, str2))
      cout << str1 <<" and " << str2 << " are permutation of each other" << endl;
    else
      cout << str1 <<" and " << str2 << " are not permutation of each other" << endl;
    str1 = "hello";
    str2 = "oellh";
     if(arePermutation_2(str1, str2))
      cout << str1 <<" and " << str2 << " are permutation of each other" << endl;
    else
      cout << str1 <<" and " << str2 << " are not permutation of each other" << endl;
    return 0;
}
/*===================================================*/
/*  ./Ch 1.Arrays And Strings/8.Zero_matrix/8.zero_matrix.cpp  */
/**
 * Cracking the coding interview 1.8
 * Write a space efficient algorithm, such that if an element in MxN is 0, the entire row and column containing it are 0.
 *
 * Approach:
 * We can use a boolean matrix of MxN or a bit vector to mark row and columns to be nullified in first iteration, but it wont be space efficient.
 * More space efficient would be to first check first row and column and if any of them contains zero, mark them to be nullified using two boolearn vars
 * let's say firstRow and firstCol, and then iterate through rest of the matrix and store information in first row column elements, only when that row
 * and column is to be marked for nullified, this way we will only change values in first row and column which are already going to be 0 in final solution. 
 */

#include <iostream>

void nullifyRow( int ** matrix, int N, int row ) {
	for ( int j = 0; j < N; ++j ) {
		matrix[row][j] = 0;
	}
}

void nullifyCol( int ** matrix, int M, int col ) {
	for ( int i = 0; i < M; ++i ) {
		matrix[i][col] = 0;
	}
}


void nullifyMatrix( int ** matrix, int M, int N ) {
	bool firstRow = false;

	//first row
	for( int i = 0; i < N; ++i ) {
		if ( matrix[0][i] == 0 ) {
			firstRow = true;
			break;
		}
	}

	//rest of the matrix
	for( int i = 1; i < M; ++i ) {
		bool nullifyThisRow = false;
		for ( int j = 0; j < N; ++j ) {
			if ( matrix[i][j] == 0 ) {
				matrix[0][j] = 0;
				nullifyThisRow = true;
			}
		}
		if (nullifyThisRow)
			nullifyRow(matrix, N, i);
	}

	//now we know which column to be nullify using information stored in previous step.
	//cols first
	for ( int j = 0; j < N; ++j ) {
		if ( matrix[0][j] == 0 ) {
			nullifyCol(matrix, M,  j);
		}
	}

	//now first row
	if ( firstRow ) {
		nullifyRow(matrix, N, 0);
	}

}

void printMatrix( int ** matrix, int M, int N ) {
	for ( int i = 0; i < M; ++i ) {
		for ( int j = 0; j < N; ++j ) {
			std::cout << matrix[i][j] << "  ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


int main()
{
	int M, N;
	std::cout << "Enter number of rows:";
	std::cin >> M;
	std::cout << "Enter number of cols:";
	std::cin >> N;
	int ** matrix = new int*[M];
	for ( int i =0; i < M; ++i ) {
		matrix[i] = new int[N];
	}
	std::cout << "Provide M x N matrix \n";
	for ( int i = 0; i < M; ++i ) {
		for ( int j = 0; j < N; ++j ) {
			std::cin >> matrix[i][j];
		}
	}

	std::cout << "Matrix Before:\n";
	printMatrix(matrix, M, N);
	nullifyMatrix(matrix, M, N);
	std::cout << "Matrix After:\n";
	printMatrix(matrix, M, N);
	return 0;
}
/*===================================================*/
/*  ./Ch 1.Arrays And Strings/1.Is Unique/1. Is_unique.cpp  */
#include <string>
#include <vector>
#include <iostream>
#include <bitset>
#include <algorithm> // for sort() 

using namespace std;

bool isUniqueChars(const string &str){
		if (str.length() > 128){
			return false;
		}
		vector<bool> char_set(128);
		for (int i = 0; i < str.length(); i++){
			int val = str[i];
			if (char_set[val]){
				return false;
			}
			char_set[val] = true;
		}
		return true;
}

bool isUniqueChars_bitvector(const string &str) {
	//Reduce space usage by a factor of 8 using bitvector. 
	//Each boolean otherwise occupies a size of 8 bits.
	bitset<256> bits(0);
	for(int i = 0; i < str.length(); i++) {
		int val = str[i];
		if(bits.test(val) > 0) {
			return false;
		}
		bits.set(val);
	}
	return true;
}
bool isUniqueChars_noDS( string str) {
	
	sort(str.begin(), str.end()); // O(nlogn) sort from <algorithm>

	bool noRepeat = true;
	for ( int i = 0 ; i < str.size() - 1; i++){
		if ( str[i] == str[i+1] ){
			noRepeat = false;
			break;
		}
	}

	return noRepeat;	
}

int main(){
		vector<string> words = {"abcde", "hello", "apple", "kite", "padle"};
		for (auto word : words)
		{
			cout << word << string(": ") << boolalpha << isUniqueChars(word) <<endl;
		}
		cout <<endl << "Using bit vector" <<endl;
		for (auto word : words)
		{
			cout << word << string(": ") << boolalpha << isUniqueChars_bitvector(word) <<endl;
		}
		cout <<endl << "Using no Data Structures" <<endl;
		for (auto word : words)
		{
			cout << word << string(": ") << boolalpha << isUniqueChars_noDS(word) <<endl;
		}
		return 0;
}

/*===================================================*/
/*  ./Ch 1.Arrays And Strings/3.URLify/URLify.cpp  */
/*
 * Cracking the coding interview Edition 6
 * Problem 1.3 URLify --> Replace all the spaces in a string with '%20'. 
 * Assumption : We have enough space to accomodate addition chars
 * Preferebly in place
 */

#include <iostream>
#include <cstring>

/*
 * Function : urlify
 * Args     : string long enough to accomodate extra chars + true len 
 * Return   : void (in place transformation of string)
 */

void urlify(char *str, int len)
{
    int numOfSpaces = 0;
    int i = 0, j = 0;
    for ( i = 0; i < len; ++i ) {
        if (str[i] == ' ') {
            ++numOfSpaces;
        }
    }
    
    int extendedLen  = len + 2 * numOfSpaces;
    i = extendedLen - 1;
    for( j = len - 1; j >= 0; --j ) {
        if ( str[j] != ' ' ) {
           str[i--] = str[j];
        } else {
            str[i--] = '0';
            str[i--] = '2';
            str[i--] = '%';
        }
    }
}

int main()
{
    char str[] = "Mr John Smith    ";                       //String with extended length ( true length + 2* spaces)
    std::cout << "Actual string   : " << str << std::endl;
    urlify(str, 13);                                        //Length of "Mr John Smith" = 13
    std::cout << "URLified string : " << str << std::endl;
    return 0;
}
/*===================================================*/
/*  ./Ch 1.Arrays And Strings/9.String_rotation/9.string_rotation.cpp  */
/**
 * Cracking the coding interview 1-9
 * You have a function "isSubstring" which checks whether a string is substring of another.
 * Given two strings s1 and s2, write code to check if s2 is a rotation of s1 using only one call to "isSubstring".
 *
 * Approach:
 * example s1 = "waterbottle", and s2 = "erbottlewat", clearly s2 is rotation of s1.
 * lets say s1 = xy ==> wat + erbottle
 * similarly s2 = yx ==> erbottle + wat
 * Therefore s1s1 = "waterbottlewaterbottle", clearly s2 is substring of s1s1
 * So if a string is formed by rotation of another string, it will always be substring of concatenated original string to itself.
 */

#include <iostream>
#include <string>

bool isRotation( std::string s1, std::string s2 ) {
	size_t len1 = s1.length();
	size_t len2 = s2.length();
	if ( len1 == 0 || len1 != len2 ) {
		return false;
	}
	std::string concatS1 = s1 + s1;
	if ( concatS1.find(s2) != std::string::npos ) {
		return true;
	} else {
		return false;
	}
}

int main() {
	std::string s1, s2;
	std::cout << "Enter string 1 : ";
	std::cin >> s1;
	std::cout << "Enter string 2 : ";
	std::cin >> s2;
	if ( isRotation(s1, s2) ) {
		std::cout << "Yes! " << s2 << " is rotation of " << s1 << std::endl;
	} else {
		std::cout << "No! " << s2 << " is not a rotation of " << s1 << std::endl;
	}
	return 0;
}

/*===================================================*/
/*  ./Ch 1.Arrays And Strings/5. One Away/5.One Away.cpp  */
#include<string>
#include<iostream>
using namespace std;

bool isOneAway(string s1, string s2){
	string a,b;
	a = s1.length() >= s2.length() ? s1 : s2;
	b = s1.length() < s2.length() ? s1 : s2;
	int len1, len2;
	len1 = a.length();
	len2 = b.length();
	if(abs(len1-len2)>1)
		return false;
	
	bool flag = false;
	for(int i=0,j=0;i<len1 && j<len2;){
		if(a[i]!=b[j]){
			if(flag)
				return false;
			flag = true;
			if(len1 == len2)
				i++,j++;
			else
				i++;
		}
		else
			i++,j++;
	}
	return true;
}

int main(void){
	string s1,s2;
	getline(cin,s1);
	getline(cin,s2);
	if(isOneAway(s1,s2))
		cout<<"One edit away."<<endl;
	else
		cout<<"Not one edit away."<<endl;
	return 0;
}
/*===================================================*/
/*  ./chapter-2-Linked-Lists/2-4-partition.cpp  */
/**
 *  Cracking the coding interview edition 6
 *  Problem 2.4 Partition:
 *  Write code to partition linked list around a value x, such that
 *  nodes less than x come before all the nodes greater than or equal to x.
 *  If x is in the list, the values of x only need to be after the elements less
 *  than x.
 *  Example
 *  3-->5-->8-->5-->10-->2-->1 (x = 5)
 *  3-->1-->2-->10-->5-->5-->8
 *
 *  Approach:
 *  Start with first node, and add every thing bigger or equal to x at tail
 *  and smaller values at head.
 */

#include <iostream>
#include <random>

struct Node {
  int data;
  Node * next;
  Node( int d ) : data{ d }, next{ nullptr } { }
};

void insert( Node * & head, int data ) {
  Node * newNode = new Node(data);
  if ( head == nullptr ) {
    head = newNode;
  } else {
    Node * curr = head;
    while( curr->next ) {
      curr = curr->next;
    }
    curr->next = newNode;
  }
}

void printList( Node * head ) {
  while ( head ) {
    std::cout << head->data << "-->";
    head = head->next;
  }
  std::cout << "nullptr" << std::endl;
}


 /* We start with a new list. Elements bigger than the pivot element are put at the tail list
 and elements smaller are put at the head list*/
Node * partition( Node * listhead , int x ) {
   Node * head = nullptr;
   Node * headInitial = nullptr;   /*The initial node of list head*/
   Node * tail = nullptr;
   Node * tailInitial = nullptr;   /*The initial node of list tail*/
   Node * curr = listhead;
   while( curr != nullptr ) {
     Node * nextNode = curr->next;
     if ( curr->data < x ) {
            if (head == nullptr) {
                head = curr;
                headInitial = head;
            }
       //insert curr node to head list
       head->next = curr;
       head = curr;
     } else {
            if (tail == nullptr) {
                tail = curr;
                tailInitial = tail;
            }
       // insert curr node to tail list
       tail->next = curr;
       tail = curr;
     }
     curr = nextNode;
   }
   head->next = tailInitial;  /*Now, we connect the head list to tail list.*/
   tail->next = nullptr;
   return headInitial;
 }





int main() {
  Node * head = nullptr;
  for ( int i = 0; i < 10; ++i ) {
		insert(head, rand() % 9);
	}
  std::cout << "List before partition around 5:\n";
  printList(head);
  std::cout << "List after partition around 5:\n";
  printList(partition(head, 5));
  return 0;
}
/*===================================================*/
/*  ./chapter-2-Linked-Lists/2-2-kthToLast.cpp  */
/**
 *  Cracking the coding interview edition 6
 *  Problem 2.2
 *  Return kth to last
 *  Implement an algorithm to find the kth to last element of a singly linked list.
 *  We can assume we are not provided the length of the list.
 *
 *  Approaches:
 *  1. Iterative:
 *  	 Use two pointers
 *  	 Move first pointer k places.
 *  	 Now move second pointer(from start) and first pointer (from k+1) simultanously.
 *  	 When second pointer would have reached end, first pointer would be at kth node.
 *
 *  2. Recursive:
 *  	 Maintain an index to keep track of node.
 *  	 Solve the problem for n-1 nodes and add 1 to index.
 *  	 Since each parent call is adding 1, when counter reaches k,
 *  	 we have reached length-k node from start, which is kth node from last.
 */

#include <iostream>

struct Node {
  int data;
  Node * next;
  Node(int d) : data{ d }, next{ nullptr } { }
};


/**
 * Insert to the head of the list
 * @param head - Current head of list
 * @param data - new node's data
 */
void insert( Node * & head, int data ) {
  Node * newNode = new Node(data);
  newNode->next = head;
  head = newNode;
}

/**
 * [deleteList - delete the entire list]
 * @param head - head of the list
 */
void deleteList( Node * & head ) {
  Node * nextNode;
  while(head) {
    nextNode = head->next;
    delete(head);
    head = nextNode;
  }
}

/**
 * printList - Helper routine to print the list
 * @param head - Current head of the list.
 */
void printList( Node * head ) {
  while(head) {
    std::cout << head->data << "-->";
    head = head->next;
  }
  std::cout << "null" << std::endl;
}

/**
 * [kthToLastHelper - helper routine to find nth node for recursive approach
 * @param  head  - head of the list
 * @param  k     - the k value for finding kth element from last of the list.
 * @param  i     - an index maintained to keep track of current node.
 * @return       - kth node from last.
 */
Node * kthToLastHelper( Node * head, int k , int & i) {
  if ( head == nullptr ) {
    return nullptr;
  }

  Node * node = kthToLastHelper(head->next, k, i);
  i = i + 1;
  //if we have solved problem k times from last.
  if ( i == k ) {
    return head;
  }
  return node;
}

/**
 * kthToLastRecursive - Recursive approach for finding kth to last element of list.
 * @param  head  - head of node
 * @param  k     - the k value for finding kth element from last of the list.
 * @return       - kth node from last.
 */
Node * kthToLastRecursive( Node * head, int k ) {
  int i = 0;
  return kthToLastHelper(head, k, i);
}

/**
 * kthToLastIterative -  Iterative approach for finding kth to last element of list.
 * @param  head  - head of node
 * @param  k     - the k value for finding kth element from last of the list.
 * @return       - kth node from last.
 */
Node * kthToLastIterative( Node * head, int k ) {
  if ( head == nullptr ) {
    return head;
  }

  Node * ptr1 = head;
  Node * ptr2 = head;

  int i = 0;
  while( i < k && ptr1 ) {
    ptr1 = ptr1->next;
    ++i;
  }

  //out of bounds
  if ( i < k ) {
    return nullptr;
  }

  while( ptr1 != nullptr ) {
    ptr1 = ptr1->next;
    ptr2 = ptr2->next;
  }

  return ptr2;
}



int main() {
  Node * head = nullptr;
  for ( int i = 7; i > 0; i-- ) {
    insert(head, i);
  }
  std::cout << "List: ";
  printList(head);

  std::cout << "4th node from last (Recursive) : ";
  Node *node4 = kthToLastRecursive(head, 4);
  if ( node4 != nullptr ) {
    std::cout << node4->data << std::endl;
  } else {
    std::cout << "NULL NODE\n";
  }

  std::cout << "4th node from last (Iterative) : ";
  node4 = kthToLastIterative(head, 4);
  if ( node4 != nullptr ) {
    std::cout << node4->data << std::endl;
  } else {
    std::cout << "NULL NODE\n";
  }

  deleteList(head);

  return 0;
}
/*===================================================*/
/*  ./chapter-2-Linked-Lists/2-6-palindrome.cpp  */
/**
 * Implement a function to check if a list is a palindrome.
 *
 * Approach 1: Reverse the half the list and compare with other half.
 * Approach 2: Iterative Approach
 * 							- Push half the list in stack,
 * 							- Compare the rest of the list by popping off from the stack
 * Approach 3: Recursive Approach
 */

#include <iostream>
#include <stack>

struct Node {
  char data;
  Node * next;
  Node ( char c ) : data{ c }, next{ nullptr } { }
};

/**
 * [insert helper routine to insert new node at head]
 * @param head [current head of the list]
 * @param c    [new node's data]
 */
void insert( Node * & head, char c ) {
  Node * newNode = new Node(c);
  newNode->next = head;
  head = newNode;
}

/**
 * [printList = helper routine to print the list]
 * @param head [head of the list]
 */
void printList( Node * head ) {
  while( head ) {
    std::cout << head->data << "-->";
    head = head->next;
  }
  std::cout << "nullptr" << std::endl;
}


/**
 * [reversedList helper routine to reverse a list]
 * @param  head [head of current list]
 * @return      [reversed list's head]
 */

void reverse( Node * & head ) {
  if ( head == nullptr  || (head && (head->next == nullptr))){
    return;
  }
  Node * newHead = nullptr;
  Node * nextNode = nullptr;
  while ( head ) {
    nextNode = head->next;
    head->next = newHead;
    newHead = head;
    head = nextNode;
  }
  head = newHead;
}


/**
 * [isPallindromeIter1 - Iteratively determine if list is palindrome using reversing the list]
 * @param  head [Head node of the list]
 * @return      [True if list is palindrome, false if not]
 */
bool isPalindromeIter1( Node * head ) {

  // if list is empty or just contains one node.
  if ( head == nullptr || head->next == nullptr ) {
    return true;
  }

  //step1 figure out middle node.
  Node * ptr1 = head;
  Node * ptr2 = head;
  Node * middleNode = nullptr;

  while( ptr2 && ptr1 && ptr1->next) {
    ptr1 = ptr1->next->next;
    ptr2 = ptr2->next;
  }

  //in case of odd number of nodes, skip the middle one
  if ( ptr1 && ptr1->next == nullptr ) {
    ptr2 = ptr2->next;
  }


  //reverse the second half of the list
  reverse(ptr2);

  middleNode = ptr2;
  // now compare the two halves
  ptr1 = head;

  while( ptr1 && ptr2 && ptr1->data == ptr2->data ) {
    ptr1 = ptr1->next;
    ptr2 = ptr2->next;
  }

  //reverse the list again.
  reverse(middleNode);

  if ( ptr2 == nullptr ) {
    return true;
  } else {
    return false;
  }
}

/**
 * [isPalindromeIter2 - Iteratively determine if list is palindrome using a stack]
 * @param  head [Head node of the list]
 * @return      [True if list is palindrome, false if not]
 */

bool isPalindromeIter2( Node * head ) {
  // if list is empty or just contains one node.
  if ( head == nullptr || head->next == nullptr ) {
    return true;
  }

  Node * ptr1 = head;
  Node * ptr2 = head;

  //pushing the first half of list to stack.
  std::stack<Node*> nodeStack;

  while( ptr2 && ptr1 && ptr1->next ) {
    ptr1 = ptr1->next->next;
    nodeStack.push(ptr2);
    ptr2 = ptr2->next;
  }

  //in case of odd number of nodes, skip the middle one
  if ( ptr1 && ptr1->next == nullptr ) {
    ptr2 = ptr2->next;
  }

  // Now compare the other half of the list with nodes
  // we just pushed in stack

  while(!nodeStack.empty() && ptr2) {
    Node * curr = nodeStack.top();
    nodeStack.pop();
    if (curr->data != ptr2->data) {
      return false;
    }
    ptr2 = ptr2->next;
  }

  return true;
}


/**
 * [isPalindromeRecurHelper - Recursive approach to determine if list is palindrome]
 * Idea is to use two pointers left and right, we move left and right to reduce
 * problem size in each recursive call, for a list to be palindrome, we need these two
 * conditions to be true in each recursive call.
 * 		a. Data of left and right should match.
 * 		b. Remaining Sub-list is palindrome.
 * We are using function call stack for right to reach at last node and then compare
 * it with first node (which is left).
 * @param  left  [left pointer of sublist]
 * @param  right [right pointer of sublist]
 * @return       [true if sublist is palindrome, false if not]
 */

bool isPalindromeRecurHelper( Node * & left, Node * right ) {
  //base case Stop when right becomes nullptr
  if ( right == nullptr ) {
    return true;
  }

  //rest of the list should be palindrome
  bool isPalindrome = isPalindromeRecurHelper(left, right->next);
  if (!isPalindrome) {
    return false;
  }

  // check values at current node.
  isPalindrome = ( left->data == right->data );

  // move left to next for next call.
  left = left->next;

  return isPalindrome;
}

bool isPalindromeRecur( Node * head ) {
  return isPalindromeRecurHelper(head, head);
}


int main()
{
  Node * head1 = nullptr;
  insert( head1, 'a' );
  insert( head1, 'b' );
  insert( head1, 'c' );
  insert( head1, 'c' );
  insert( head1, 'b' );
  insert( head1, 'a' );
  std::cout << "List 1: ";
  printList(head1);
  if ( isPalindromeIter1(head1) ) {
    std::cout << "List 1 is pallindrome list\n";
  } else {
    std::cout << "List 1 is not a pallindrome list\n";
  }
  std::cout << "List 1: ";
  printList(head1);

  Node * head2 = nullptr;
  insert( head2, 'r');
  insert( head2, 'a');
  insert( head2, 'd');
  insert( head2, 'a');
  insert( head2, 'r');
  std::cout << "List 2: ";
  printList(head2);

  if ( isPalindromeIter2( head2 ) ) {
    std::cout << "List 2 is pallindrome list\n";
  } else {
    std::cout << "List 2 is not a pallindrome list\n";
  }

  std::cout << "List 2: ";
  printList(head2);

  Node * head = nullptr;
  insert( head, 'a' );
  insert( head, 'b' );
  insert( head, 'c' );
  insert( head, 'b' );
  insert( head, 'd' );
  std::cout << "List 3: ";
  printList(head);

  if ( isPalindromeRecur(head) ) {
    std::cout << "List 3 is pallindrome list\n";
  } else {
    std::cout << "List 3 is not a pallindrome list\n";
  }
  std::cout << "List 3: ";
  printList(head);
  return 0;
}
/*===================================================*/
/*  ./chapter-2-Linked-Lists/2-8-loop-detection.cpp  */
/**
 *  Cracking the coding interview, edition 6
 *  Problem 2.8 Loop Detection
 *  Problem : Determine if a linkedlist has a loop. If it has a loop, find the start of loop.
 *  NOTE: I have followed the approach provided in book, however, once I find start of loop,
 *  I remove the loop. So that we can test our solution. Read comment at line 25.
 */

#include <iostream>

struct Node {
  int data;
  Node * next;
  Node( int d ) : data{ d }, next{ nullptr } { }
};

void removeLoop( Node * loopNode, Node * head )
{
  Node * ptr1 = head;
  Node * ptr2 = loopNode;
  while ( ptr1->next != ptr2->next ) {
    ptr1 = ptr1->next;
    ptr2 = ptr2->next;
  }
  //ptr2 has reached start of loop, now removing the loop.
  ptr2->next = nullptr;
}

bool detectAndRemoveCycle( Node * head )
{
  if ( head == nullptr) {
    return false;
  }
  Node * fastPtr = head;
  Node * slowPtr = head;
  while( slowPtr && fastPtr && fastPtr->next)
  {
    fastPtr = fastPtr->next->next;
    slowPtr = slowPtr->next;
    if ( fastPtr == slowPtr ) {
      removeLoop( slowPtr, head );
      return true;
    }
  }
  return false;
}



void insert( Node * & head, int data )
{
  Node * newNode = new Node( data );
  if ( head == nullptr ) {
    head = newNode;
  } else {
    Node * temp = head;
    while( temp->next != nullptr ) {
      temp = temp->next;
    }
    temp->next = newNode;
  }
}

void printList( Node * head )
{
  while( head ) {
    std::cout << head->data << "-->";
    head = head->next;
  }
  std::cout << "NULL" << std::endl;
}


int main()
{
    Node * head = nullptr;
    insert( head , 1 );
    insert( head , 2 );
    insert( head , 3 );
    insert( head , 4 );
    insert( head , 5 );
    std::cout << "Current List:\n";
    printList( head );
    std::cout << "Inserting loop, connecting 5 to 2 \n";
    head->next->next->next->next->next = head->next;
    std::cout << "Detecting and deleting loop\n";
    detectAndRemoveCycle(head);
    std::cout << "Back to the same old list\n";
    printList( head );
    return 0;
}
/*===================================================*/
/*  ./chapter-2-Linked-Lists/2-7-intersection.cpp  */
/**
 * Cracking the coding interview edition 6
 * Problem 2.7 Intersection
 * Given two linked lists, if they both intersect at some point.
 * Find out the intersecting point else return nullptr.
 * Intersection is defined based on reference not value.
 */

#include <iostream>
#include <cmath>

struct Node {
  int data;
  Node * next;
  Node( int d ) : data{ d }, next{ nullptr } { }
};

/**
 * [printList Helper routine to print list]
 * @param head [head of the list]
 */
void printList( Node * head )
{
  while( head ) {
    std::cout << head->data << "-->";
    head = head->next;
  }
  std::cout << "NULL" << std::endl;
}

int listLen( Node * head )
{
  int count = 0;
  while( head ) {
    head = head->next;
    count++;
  }
  return count;
}

/**
 * [intersectionPoint Returns the point of intersection of two lists]
 * @param  head1 [ head of list 1 ]
 * @param  head2 [ head of list 2 ]
 * @return       [ Intersecting node, if lists intersect, else nullptr]
 */
Node * intersectionPoint( Node * head1, Node * head2 )
{
  int len1 = listLen(head1);
  int len2 = listLen(head2);
  //figure out the bigger list ( and smaller )
  //ptr points to bigger list, let us move the difference
  //between the two.
  Node * ptr1 = ( len1 > len2 ) ? head1 : head2;
  Node * ptr2 = ( len1 > len2 ) ? head2 : head1;
  int i = 0;
  while ( i < std::abs(len1 - len2) && ptr1 ) {
    ptr1 = ptr1->next;
    ++i;
  }
  //Now we have equal nodes to travel on both the nodes
  // traversing and comparing the pointers.

  while( ptr1 && ptr2 ) {
    if ( ptr1 == ptr2 ) {
      return ptr1;
    }
    ptr1 = ptr1->next;
    ptr2 = ptr2->next;
  }
  return nullptr;
}


int main()
{
  Node * list1 = new Node(3);
  list1->next = new Node(6);
  list1->next->next = new Node(9);
  list1->next->next->next = new Node(12);
  list1->next->next->next->next = new Node(15);
  list1->next->next->next->next->next = new Node(18);

  Node * list2 = new Node(7);
  list2->next = new Node(10);
  list2->next->next = list1->next->next->next;

  printList(list1);
  printList(list2);

  Node * intersectingNode = intersectionPoint( list1 , list2 );
  if (intersectingNode) {
    std::cout << "Intersecting Node of lists is :" << intersectingNode->data << std::endl;
  } else {
    std::cout << "Lists do not interset" << std::endl;
  }
  return 0;
}
/*===================================================*/
/*  ./chapter-2-Linked-Lists/2-1-remove-dups.cpp  */
/*
 * Cracking the coding interview edition 6
 * Problem 2-1 : Remove duplicates from an unsorted linked list.
 * Approach 1 : Naive approach of iterating and remove all further duplicates of current node.
 * 							Space complexity O(1) & time complexity O(n^2)
 * Approach 2: Use a hash table, space complexity O(n), time complexity O(n)
 */


#include <iostream>
#include <unordered_map>
#include <random>


struct Node {
	int data = 0;
	Node * next = nullptr;
};

/**
 * [insert - insert a node at the head of list]
 * @param head [head of the list]
 * @param data [new node's data]
 */
void insert( Node * & head, int data )
{
	Node * newNode = new Node;
	newNode->data = data;
	newNode->next = head;
	head = newNode;
}

/**
 * [printList Helper routine to print list]
 * @param head [head of the list]
 */
void printList( Node * head ) {
	while( head ) {
		std::cout << head->data << "-->";
		head = head->next;
	}
	std::cout << "nullptr" << std::endl;
}

//generate a random int between min and max
/**
 * [random_range helper routine to generate a random number between min and max (including)]
 * @param  min [min of range]
 * @param  max [max of range]
 * @return     [A random number between min and max]
 */
static inline int random_range(const int min, const int max) {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(mt);
}


// Method 1
//space complexity O(1)
// time complexity O(n^2)
/**
 * [removeDuplicates Remove duplicates without using extra space]
 * @param head [head of list]
 */
void removeDuplicates( Node * head ) {
	if ( head == nullptr || ( head && (head->next == nullptr))) {
		return;
	}
	Node * curr = head;
	while(curr) {
		Node * runner = curr;
		while (runner->next != nullptr) {
			if (runner->next->data == curr->data) {
				runner->next = runner->next->next;
			} else {
				runner = runner->next;
			}
		}
		curr = curr->next;
	}
}

// Method 2
// space complexity - O(n)
// time complexity - O(n)
/**
 * [removeDuplicates1 - Remove duplicates from the list using hash table]
 * @param head [head of list]
 */
void removeDuplicates1( Node * head ) {
	if ( head == nullptr || ( head && (head->next == nullptr) )) {
		return ;
	}
	std::unordered_map<int, int> node_map;
	Node * prev = head;
	Node * curr = head->next;
	node_map[head->data] = 1;
	while( curr != nullptr ) {
		while (curr && node_map.find(curr->data) != node_map.end()) {
			curr = curr->next;
		}
		prev->next = curr;
		prev = curr;
		if (curr) {
			node_map[curr->data] = 1;
			curr = curr->next;
		}
	}
}



int main() {
	std::cout << "Method 1 : \n";
	Node * head = nullptr;
	for ( int i = 0; i < 10; ++i ) {
		insert(head, random_range(1,7));
	}
	printList(head);
	removeDuplicates(head);
	printList(head);

	std::cout << "Method 2 : \n";
	Node * head1 = nullptr;
	for ( int i = 0; i < 10; ++i ) {
		insert(head1, random_range(1,7));
	}
	printList(head1);
	removeDuplicates1(head1);
	printList(head1);
	return 0;
}
/*===================================================*/
/*  ./chapter-2-Linked-Lists/2-5-add-lists.cpp  */
/**
 * Cracking the coding interview edition 6
 * Problem 2.5 Sum lists
 * You have two numbers represented by linked list, where each node contains
 * a single digit. Digits are stored in reverse order.(1's digit is at the head)
 * Write a function that adds two such numbers and returns a number in similar
 * list format.
 * example:
 * 7-->1-->6 + 5-->9-->2 = 2-->1-->9
 * which is (617 + 295  = 912)
 * What if digits are not stored in reverse order(i.e 1's digit is at tail)
 * (6--1-->7) + (2-->9-->5) = (9-->1-->2)
 *
 * Approach:
 * We will solve the problem recursively and iteratively.
 * Add numbers at same digits place, store the 1's digit of the output in new list
 * and add carry in next place's addition.
 *
 * Finally, we will solve the follow up.
 */

#include <iostream>

struct Node {
  int data;
  Node * next;
  Node ( int d ) : data{ d }, next{ nullptr } { }
};

/**
 * [insert - insert a new node at head of the list]
 * @param head [head of the list]
 * @param data [new node's data]
 */
void insert( Node * & head, int data ) {
  Node * newNode = new Node(data);
  newNode->next = head;
  head = newNode;
}

/**
 * [printList - print the list]
 * @param head [head of the list]
 */
void printList( Node * head ) {
  while ( head ) {
    std::cout << head->data << "-->";
    head = head->next;
  }
  std::cout << "nullptr" << std::endl;
}

/**
 * [add_iterative iterative approach to add two given lists]
 * @param  list1
 * @param  list2
 * @return list3
 */
Node * add_iterative( Node * list1, Node * list2 ) {
  if ( list1 == nullptr ) {
    return list2;
  }
  if ( list2 == nullptr ) {
    return list1;
  }

  // list3 will store result
  Node * list3 = nullptr;
  // for adding new nodes to tail of list3
  Node * list3Tail = nullptr;

  int value = 0, carry = 0;

  while( list1 || list2 ) {
    // add the values, if one of the list has already been traversed, add 0
    value = carry + (list1 ? list1->data : 0 ) + (list2 ? list2->data : 0);

    // get the new value and carry
    if ( value > 9 ) {
      carry = 1;
      value = value % 10;
    } else {
      carry = 0;
    }

    //new node
    Node * temp = new Node(value);

    //if this is the first node, populate the result, else add to the tail
    if ( list3 == nullptr ) {
      list3 = temp;
    } else {
      list3Tail->next = temp;
    }

    //make new tail
    list3Tail = temp;

    if (list1) {
      list1 = list1->next;
    }

    if (list2) {
      list2 = list2->next;
    }
  }

  if ( carry > 0 ) {
    list3Tail->next = new Node(carry);
  }
  return list3;
}

/**
 * [add_recursive - recursive addititon of two lists
 * @param  list1
 * @param  list2
 * @param  carry
 * @return list3
 */
Node * add_recursive( Node * list1, Node * list2, int carry) {
  if ( list1 == nullptr && list2 == nullptr && carry == 0 ) {
    return nullptr;
  }
  int value = carry;
  if ( list1 ) {
    value += list1->data;
  }
  if ( list2 ) {
    value += list2->data;
  }

  Node * resultNode = new Node( value % 10 );

  resultNode->next = add_recursive( list1 ? (list1->next) : nullptr,
                                    list2 ? (list2->next) : nullptr,
                                    value > 9 ? 1 : 0 );
  return resultNode;
}

/**
 *  Follow up part:
 *  Lists are stored such that 1's digit is at the tail of list.
 *  617 ==> 6 --> 1 --> 7
 *  295 ==> 2 --> 9 --> 5
 */

/**
 * [padList - Helper routine for padding the shorter list]
 * @param list    [Current list]
 * @param padding [number of padding required]
 */
void padList( Node * & list, int padding ) {
  for ( int i = 0; i < padding; ++i ) {
    insert(list, 0);
  }
}

/**
 * [length - helper routine to return length of list]
 * @param  head [list's head]
 * @return length of the list
 */
int length( Node * head ) {
  int len = 0;
  while( head ) {
    len++;
    head = head->next;
  }
  return len;
}

Node * add_followup_helper( Node * list1, Node * list2, int & carry ) {
  if ( list1 == nullptr && list2 == nullptr && carry == 0 ) {
    return nullptr;
  }

  Node * result = add_followup_helper(list1 ? (list1->next) : nullptr,
                                      list2 ? (list2->next) : nullptr,
                                      carry);

  int value = carry + (list1 ? list1->data : 0 ) + (list2 ? list2->data : 0);
  insert( result, value % 10 );
  carry = ( value > 9 ) ? 1 : 0;
  return result;
}

/**
 * [add_followup - adding list such that 1's digit is at tail( follow up part of question)
 * @param  list1
 * @param  list2
 * @return list3 representing sum of list1 and list2
 */
Node * add_followup( Node * list1, Node * list2 ) {
  int len1 = length(list1);
  int len2 = length(list2);

  //pad the smaller list
  if ( len1 > len2 ) {
    padList( list2, len1 - len2 );
  } else {
    padList( list1, len2 - len1 );
  }
  int carry = 0;
  Node * list3 = add_followup_helper( list1, list2, carry);
  if ( carry ) {
    insert( list3, carry);
  }
  return list3;
}

/**
 * [deleteList Helper routine to delete list]
 * @param head [head of the list]
 */
 void deleteList( Node * & head ) {
   Node * nextNode;
   while(head) {
     nextNode = head->next;
     delete(head);
     head = nextNode;
   }
 }

int main()
{
  //making list 1 for number 617
  Node * list1 = nullptr;
  insert(list1, 6);
  insert(list1, 1);
  insert(list1, 7);
  std::cout << "List1:  ";
  printList(list1);

  //making list2 for number 295
  Node * list2 = nullptr;
  insert(list2, 2);
  insert(list2, 9);
  insert(list2, 5);
  std::cout << "List2:  ";
  printList(list2);

  Node * list3 = add_iterative(list1, list2);
  std::cout << "Iterative Solution: \n";
  std::cout << "List3:  ";
  printList(list3);

  Node * list4= add_recursive(list1, list2, 0);
  std::cout << "Recursive Solution: \n";
  std::cout << "List4:  ";
  printList(list4);

  deleteList(list1);
  deleteList(list2);
  deleteList(list3);
  deleteList(list4);

  std::cout << "\n\nNow follow up case, lists are stored such that 1's digit is at the tail of list\n";
  //Node * listx = nullptr;
  insert(list1, 4);
  insert(list1, 3);
  insert(list1, 2);
  insert(list1, 9);
  std::cout << "List1:  ";
  printList(list1);

  insert(list2, 9);
  insert(list2, 9);
  insert(list2, 8);
  std::cout << "List2:  ";
  printList(list2);

  list3 = add_followup(list1, list2);
  std::cout << "Adding two above lists\n";
  std::cout << "List3:  ";
  printList(list3);

  deleteList(list1);
  deleteList(list2);
  deleteList(list3);

  return 0;
}
/*===================================================*/
/*  ./chapter-2-Linked-Lists/2-3-delete-middle-node.cpp  */
/**
 * Cracking the coding interview - edition 6
 * Problem 2.3 Delete middle node:
 * Implement an algorithm to delete a node in the middle of a singly linked list.
 * We are given pointer to that node.
 *
 * Approach:
 * In order to remove a node 'A' from a list, We will need to connect pointer of
 * A's previous node to A's next node. Here we don't have access to previous node.
 * However, we have pointer to that node, we can copy the data of next node to
 * the pointed node and then remove the next node.
 * Assumption here is that we are not given last node of the list for deletion.
 */

#include <iostream>

struct Node {
  char data;
  Node * next;
  Node( char c ) : data{ c }, next{ nullptr } { }
};

/**
 * [printList - Helper routine to print the list]
 * @param head [head of the list]
 */
void printList( Node * head ) {
  while( head ) {
    std::cout << head->data << "-->";
    head = head->next;
  }
  std::cout << "nullptr" << std::endl;
}

/**
 * [deleteNode - delete the "node" from the list]
 * @param node [node to be deleted]
 */
void deleteNode( Node * node ) {
  if ( node == nullptr || node->next == nullptr ) {
    return;
  }
  Node * nextNode = node->next;
  node->data = nextNode->data;
  node->next = nextNode->next;
  delete nextNode;
}

int main() {
  Node * head = new Node('a');
  head->next = new Node('b');
  head->next->next = new Node('c');
  head->next->next->next = new Node('d');
  head->next->next->next->next = new Node('e');
  std::cout << "List before deletion:\n";
  printList(head);
  std::cout << "Deleting node containing data as 'c'\n";
  deleteNode(head->next->next);
  std::cout << "List after deletion:\n";
  printList(head);
  return 0;
}
/*===================================================*/
/*  ./Chapter-3-Stacks-and-Queues/3.1-Three-in-One/FixedMultiStack.h  */
#include <iostream>
#include <cstdlib>

class FixedMultiStack
{
public:
  //Create 3 stacks, each stack is of size stackCapacity.
  FixedMultiStack(int stackCapacity);
  virtual ~FixedMultiStack();

  //Push an element onto stack stackNum, where stackNum is from 0 to 2.
  void push(int stackNum, int value);

  //Pop the top element from stack stackNum, where stackNum is from 0 to 2.
  void pop(int stackNum);

  //Return the top element on stack stackNum, where stackNum is from 0 to 2.
  int top(int stackNum) const;

  bool isEmpty(int stackNum) const;
  bool isFull(int stackNum) const;

private:
  int numOfStack = 3;
  int stackCapacity;
  int *stackArray;
  int *stackCapacityUsed;

  //Return the top index of stack stackNum, where stackNum is from 0 to 2.
  int indexOfTop(int stackNum) const;
};
/*===================================================*/
/*  ./Chapter-3-Stacks-and-Queues/3.1-Three-in-One/FixedMultiStack.cpp  */
#include "FixedMultiStack.h"

FixedMultiStack::FixedMultiStack(int stackCapacity)
{
  this->stackCapacity = stackCapacity;
  stackArray = new int[numOfStack * stackCapacity]();
  stackCapacityUsed = new int[numOfStack]();
}

FixedMultiStack::~FixedMultiStack()
{
  delete [] stackArray;
  delete [] stackCapacityUsed;
}

void FixedMultiStack::push(int stackNum, int value)
{
  if(isFull(stackNum))
    std::cout << "Stack " << stackNum << " is full.\n";
  else
  {
    stackCapacityUsed[stackNum]++;
    stackArray[indexOfTop(stackNum)] = value;
  }
}

void FixedMultiStack::pop(int stackNum)
{
  if(isEmpty(stackNum))
    std::cout << "Stack " << stackNum << " is empty.\n";
  else
  {
    int topIndex = indexOfTop(stackNum);
    stackArray[topIndex] = 0;
    stackCapacityUsed[stackNum]--;
  }
}

int FixedMultiStack::top(int stackNum) const
{
  if(isEmpty(stackNum))
  {
    std::cout << "Stack " << stackNum << " is empty.\n";
    exit(1); //Or throw an exception.
  }
  else
    return stackArray[indexOfTop(stackNum)];
}

bool FixedMultiStack::isEmpty(int stackNum) const
{
  return (stackCapacityUsed[stackNum] == 0);
}

bool FixedMultiStack::isFull(int stackNum) const
{
  return (stackCapacityUsed[stackNum] == stackCapacity);
}

int FixedMultiStack::indexOfTop(int stackNum) const
{
  int startIndex = stackNum * stackCapacity;
  int capacity = stackCapacityUsed[stackNum];
  return (startIndex + capacity - 1);
}
/*===================================================*/
/*  ./Chapter-3-Stacks-and-Queues/C++14/3.6-AnimalShelter.cpp  */
// Animal Shelter: An animal shelter, which holds only dogs and cats operates on a strictly "first in, first
// out" basis. People must adopt either the "oldest" (based on arrival time) of all animals at the shelter,
// or they can select whether they would prefer a dog or a cat (and will receive the oldest animal of
// that type). They cannot select which specific animal they would like. Create the data structures to
// maintain this system and implement operations such as enqueue, dequeueAny, dequeueDog,
// and dequeueCat. You may use the built-in LinkedList data structure.

#include <iostream>
#include <limits>
#include <memory>
#include "queue.hpp"

class Animal
{
protected:
    Animal(std::string &&animalName) : name(std::move(animalName))
    {
        orderNo = std::numeric_limits<decltype(orderNo)>::max();
    }

public:
    virtual ~Animal()
    {
    }

    void setOrder(size_t order)
    {
        orderNo = order;
    }

    size_t getOrder() const
    {
        return orderNo;
    }

    const std::string &getName() const
    {
        return name;
    }

    bool operator < (const Animal &other)
    {
        return orderNo < other.orderNo;
    }

    template <typename T>
    static std::shared_ptr<Animal> create(std::string &&name)
    {
        return std::make_shared<T>(std::move(name));
    }

private:
    std::string name;
    size_t orderNo;
};

class Dog: public Animal
{
public:
    Dog(std::string &&name) : Animal(std::move(name))
    {
    }
};

class Cat: public Animal
{
public:
    Cat(std::string &&name) : Animal(std::move(name))
    {
    }
};

class Shelter
{
public:
    Shelter() : nextOrderNo(0)
    {
    }

    void enqueue(std::shared_ptr<Animal> &&animal)
    {
        if (auto dog = std::dynamic_pointer_cast<Dog>(animal))
        {
            dog->setOrder(nextOrderNo++);
            dogs.add(std::move(dog));
        }
        else if (auto cat = std::dynamic_pointer_cast<Cat>(animal))
        {
            cat->setOrder(nextOrderNo++);
            cats.add(std::move(cat));
        }
        else
            throw BadAnimalException();
    }

    std::shared_ptr<Animal> dequeueAny()
    {
        if (dogs.isEmpty())
            return dequeueCat();
        else if (cats.isEmpty())
            return dequeueDog();
        else if (*dogs.peek() < *cats.peek())
            return dequeueDog();
        else
            return dequeueCat();
    }

    std::shared_ptr<Animal> dequeueCat()
    {
        return std::static_pointer_cast<Animal>(cats.remove());
    }

    std::shared_ptr<Animal> dequeueDog()
    {
        return std::static_pointer_cast<Animal>(dogs.remove());
    }

    class BadAnimalException {};

private:
    Queue<std::shared_ptr<Dog>> dogs;
    Queue<std::shared_ptr<Cat>> cats;
    size_t nextOrderNo;
};

int main()
{
    Shelter shelter;
    for (auto name : {"Dog 1", "Cat 1", "Dog 2", "Dog 3 ", "Cat 2", "Cat 3", "Cat 4", "Dog 4", "Dog 5", "Dog 6", "Cat 5", "Cat 6", "Dog 7", "Dog 8", "Cat 7", "Dog 9"})
    {
        if (name[0] == 'D')
            shelter.enqueue(Animal::create<Dog>(std::move(name)));
        else if (name[0] == 'C')
            shelter.enqueue(Animal::create<Cat>(std::move(name)));
    }

    std::cout << "any --> " << shelter.dequeueAny()->getName() << std::endl;
    std::cout << "any --> " << shelter.dequeueAny()->getName() << std::endl;

    std::cout << "dog --> " << shelter.dequeueDog()->getName() << std::endl;
    std::cout << "cat --> " << shelter.dequeueCat()->getName() << std::endl;
    std::cout << "cat --> " << shelter.dequeueCat()->getName() << std::endl;
    std::cout << "cat --> " << shelter.dequeueCat()->getName() << std::endl;
    std::cout << "cat --> " << shelter.dequeueCat()->getName() << std::endl;
    std::cout << "dog --> " << shelter.dequeueDog()->getName() << std::endl;
    std::cout << "dog --> " << shelter.dequeueDog()->getName() << std::endl;
    std::cout << "cat --> " << shelter.dequeueCat()->getName() << std::endl;
    std::cout << "any --> " << shelter.dequeueAny()->getName() << std::endl;
    return 0;
}/*===================================================*/
/*  ./Chapter-3-Stacks-and-Queues/C++14/3.3-StackOfPlatesFU.cpp  */
// Stack of Plates
// FOLLOW UP
// Implement a function popAt(int index) which performs pop operation on a specified sub-stack.

#include <iostream>
#include <deque>

template <typename T, size_t Capacity>
class SetOfStacks
{
public:
    template<typename U>
    void push(U &&value)
    {
        if (stacks.empty() || stacks.back().size() >= Capacity)
            stacks.emplace_back(1, std::forward<U>(value));
        else
            stacks.back().push_back(std::forward<U>(value));
    }

    T &peek()
    {
        return stacks.back().back();
    }

    T pop()
    {
        T value = stacks.back().back();
        stacks.back().pop_back();
        if (stacks.back().empty())
            stacks.pop_back();
        return value;
    }

    // O(N)
    T popAt(int index)
    {
        if (index < 0 || index >= stacks.size())
            throw OutOfIndexException();
        T value = stacks[index].back();
        stacks[index].pop_back();
        shiftLeftTo(index);
        return value;
    }

    // Number of used limited stacks
    size_t size() const
    {
        return stacks.size();
    }

    class OutOfIndexException
    {
    };


private:

    void shiftLeftTo(int index)
    {
        if (index == stacks.size() - 1)
        {
            // last stack
            if (stacks.back().empty())
                stacks.pop_back();
        }
        else
        {
            stacks[index].push_back(stacks[index + 1].front());
            stacks[index + 1].pop_front();
            shiftLeftTo(index + 1);
        }
    }

    std::deque<std::deque<T>> stacks;
};

// Special case if capacity is 1. In this case popAt(int index) makes a specified sub-stack empty,
// it should be then removed instead of shifting left values.
template <typename T>
class SetOfStacks<T, 1>
{
public:
    template<typename U>
    void push(U &&value)
    {
        stacks.push_back(std::forward<T>(value));
    }

    T &peek()
    {
        return stacks.at(stacks.size() - 1);
    }

    T pop()
    {
        T value = stacks.back();
        stacks.pop_back();
        return value;
    }

    T popAt(int index)
    {
        if (index < 0 || index >= stacks.size())
            throw OutOfIndexException();
        T value = stacks.at(index);
        stacks.erase(stacks.begin() + index);
        return value;
    }

    size_t size() const
    {
        return stacks.size();
    }

    class OutOfIndexException
    {
    };


private:

    void shiftLeftTo(int index)
    {
        if (index == stacks.size() - 1)
        {
            // last stack
            if (stacks.back().empty())
                stacks.pop_back();
        }
        else
        {
            stacks[index].push_back(stacks[index + 1].front());
            stacks[index + 1].pop_front();
            shiftLeftTo(index + 1);
        }
    }

    std::deque<T> stacks;
};


// Forbid capacity 0
template <typename T>
class SetOfStacks<T, 0>
{
public:
    SetOfStacks() = delete;
};

int main()
{
    SetOfStacks<int, 2> stack;
    for (int i = 0; i < 13; ++i)
    {
        stack.push(i);
        std::cout << i << " is pushed into the stack " << stack.size() << std::endl;
    }

    std::cout << std::endl;
    stack.popAt(4);
    stack.popAt(4);

    while (stack.size() != 0)
    {
        size_t stackNo = stack.size();
        std::cout << stack.pop() << " is popped from the stack " << stackNo << std::endl;
    }

    std::cout << std::endl;

    SetOfStacks<int, 1> stack1;
    for (int i = 0; i < 13; ++i)
    {
        stack1.push(i);
        std::cout << i << " is pushed into the stack " << stack1.size() << std::endl;
    }

    std::cout << std::endl;
    stack1.popAt(4);
    stack1.popAt(4);

    while (stack1.size() != 0)
    {
        size_t stackNo = stack1.size();
        std::cout << stack1.pop() << " is popped from the stack " << stackNo << std::endl;
    }
    return 0;
}
/*===================================================*/
/*  ./Chapter-3-Stacks-and-Queues/C++14/3.5-SortStack.cpp  */
// Sort Stack: Write a program to sort a stack such that the smallest items are on the top. You can use
// an additonal temporary stack, but you may not copy the elements into any other data struture
// (such as an array). The stack support the following operations: pop, peek, and isEmpty.

#include <iostream>
#include "stack.hpp"

template <typename T>
class SortedStack
{
public:
    SortedStack() : sorted(false)
    {
    }

    template <typename U>
    void push(U &&value)
    {
        stack.push(std::forward<U>(value));
        sorted = false;
    }

    T &peek()
    {
        sort();
        return stack.peek();
    }

    T pop()
    {
        sort();
        return stack.pop();
    }

    bool isEmpty() const
    {
        return stack.isEmpty();
    }

private:
    void sort()
    {
        if (sorted)
            return;

        Stack<T> helper;
        while (!stack.isEmpty())
        {
            T value = stack.pop();

            // Move greater than 'value' elements from 'helper' to 'stack'
            while (!helper.isEmpty() && value < helper.peek())
                stack.push(helper.pop());

            // Place 'value' above smaller element into 'helper'
            helper.push(std::move(value));
        }

        // Copy from 'helper' into 'stack' in reversed order
        while (!helper.isEmpty())
            stack.push(helper.pop());
        sorted = true;
    }

    Stack<T> stack;
    bool sorted;
};

int main()
{
    SortedStack<int> stack;
    for (auto v : {5, 10, 4, 9, 3, 3, 8, 1, 2, 2, 7, 6})
    {
        stack.push(v);
        std::cout << "Pushed value: " << v << std::endl;
    }

    std::cout << std::endl;

    while (!stack.isEmpty())
    {
        auto v = stack.pop();
        std::cout << "Popped value: " << v << std::endl;
    }
    return 0;
}/*===================================================*/
/*  ./Chapter-3-Stacks-and-Queues/C++14/3.2-StackMin.cpp  */
// StackMin: How would you design a stack which, in addition to push and pop, has a function min
// which returns the minimum element? Push, pop and min should all operate in O(1) time.

#include <iostream>
#include "stack.hpp"

template<typename T>
class StackMin
{
public:
    template<typename U>
    void push(U &&value)
    {
        if (minStack.isEmpty() || value <= minStack.peek())
            minStack.push(value);
        stack.push(std::forward<U>(value));
    }

    T &peek()
    {
        return stack.peek();
    }

    T &min()
    {
        return minStack.peek();
    }

    T pop()
    {
        auto value = stack.pop();
        if (value == min())
            minStack.pop();
        return value;
    }

    bool isEmpty()
    {
        return stack.isEmpty();
    }

private:
    Stack<T> stack;
    Stack<T> minStack;
};

int main()
{
    StackMin<int> stack;
    for (auto v : {5, 10, 4, 9, 3, 3, 8, 2, 2, 7, 6})
    {
        stack.push(v);
        std::cout << "Pushed value: " << v << ", min value: " << stack.min() << std::endl;
    }
    while (!stack.isEmpty())
    {
        auto v = stack.pop();
        std::cout << "Popped value: " << v;
        if (stack.isEmpty())
            std::cout << ", stack is empty" << std::endl;
        else
            std::cout << ", min value: " << stack.min() << std::endl;
    }
    return 0;
}/*===================================================*/
/*  ./Chapter-3-Stacks-and-Queues/C++14/3.4-QueueViaStacks.cpp  */
// Queue via Stacks: Implement a MyQueue class which implements a queue using two stacks.

#include <iostream>
#include "stack.hpp"

template <typename T>
class MyQueue
{
public:
    template <typename U>
    void add(U &&value)
    {
        newValues.push(std::forward<U>(value));
    }

    T peek()
    {
        if (reversed.isEmpty())
            move(newValues, reversed);
        return reversed.peek();
    }

    T remove()
    {
        if (reversed.isEmpty())
            move(newValues, reversed);
        return reversed.pop();
    }

    bool isEmpty()
    {
        return newValues.isEmpty() && reversed.isEmpty();
    }

private:
    static void move(Stack<T> &from, Stack<T> &to)
    {
        while (!from.isEmpty())
            to.push(from.pop());
    }
    Stack<T> newValues;
    Stack<T> reversed;
};

int main()
{
    MyQueue<int> queue;
    for (int i = 0; i < 10; ++i)
    {
        queue.add(i);
        std::cout << "Queued value " << i << std::endl;
    }

    for (int i = 0; i < 5; ++i)
        std::cout << "Removed value " << queue.remove() << std::endl;

    for (int i = 10; i < 15; ++i)
    {
        queue.add(i);
        std::cout << "Queued value " << i << std::endl;
    }

    while (!queue.isEmpty())
        std::cout << "Removed value " << queue.remove() << std::endl;

    return 1;
}/*===================================================*/
/*  ./Chapter-3-Stacks-and-Queues/C++14/3.3-StackOfPlates.cpp  */
// Stack of Plates: Imagine a (literal) stack of plates. If the stack gets too high, it might topple.
// Therefore, in real life, we would likely start a new stack when the previous stack exceeds some
// threshold. Implement a data structure SetOfStacks that mimics this. SetOfStacks should be
// composed of several stacks and should create a new stack once a previous one exceeds capacity.
// SetOfStacks.push() and SetOfStacks.pop() should behave identically to a singles stack
// (that is, pop() should return the same values as it would if there just a single stack).

#include <iostream>
#include "stack.hpp"

template <typename T, size_t Capacity>
class SetOfStacks
{
public:
    template<typename U>
    void push(U &&value)
    {
        if (stacks.isEmpty() || stacks.peek().size() >= Capacity)
            stacks.push(Stack<T>()); // start new stack
        stacks.peek().push(std::forward<U>(value));
    }

    T &peek()
    {
        return stacks.peek().peek();
    }

    T pop()
    {
        T value = stacks.peek().pop();
        if (stacks.peek().isEmpty())
            stacks.pop();
        return value;
    }

    // Number of limited stacks
    size_t size() const
    {
        return stacks.size();
    }

private:
    Stack<Stack<T>> stacks;
};

// If Capacity is 1 we do not need stack of stacks.
template <typename T>
class SetOfStacks<T, 1> : public Stack<T>
{
};

// Forbid Capacity 0
template <typename T>
class SetOfStacks<T, 0>
{
public:
    SetOfStacks() = delete;
};

int main()
{
    SetOfStacks<int, 2> stack;

    for (int i = 0; i < 11; ++i)
    {
        stack.push(i);
        std::cout << i << " is pushed into the stack " << stack.size() << std::endl;
    }

    std::cout << std::endl;

    while (stack.size() != 0)
    {
        size_t stackNo = stack.size();
        std::cout << stack.pop() << " is popped from the stack " << stackNo << std::endl;
    }

    std::cout << std::endl;

    SetOfStacks<int, 1> stack1;

    for (int i = 0; i < 11; ++i)
    {
        stack1.push(i);
        std::cout << i << " is pushed into the stack " << stack1.size() << std::endl;
    }

    std::cout << std::endl;

    while (!stack1.isEmpty())
    {
        size_t stackNo = stack1.size();
        std::cout << stack1.pop() << " is popped from the stack " << stackNo << std::endl;
    }

    return 0;
}/*===================================================*/
/*  ./Chapter-3-Stacks-and-Queues/3.2-Stack-Min/Stack.cpp  */
#include "Stack.h"

Stack::Stack()
{
  head = nullptr;
  stackSize = 0;
}

Stack::~Stack()
{
  StackNode *discard;
  while(head != nullptr)
  {
    discard = head;
    head = head->next;
    delete discard;
  }
}

void Stack::push(int item)
{
  if(isEmpty())
  {
    head = new StackNode(item, nullptr);
    head->minimum = head;
  }
  else if(item <= head->minimum->data)
  {
    head = new StackNode(item, head);
    head->minimum = head;
  }
  else
  {
    head = new StackNode(item, head);
    head->minimum = head->next->minimum;
  }

  stackSize++;
}

void Stack::pop()
{
  if(stackSize == 0 || head == nullptr)
    return;

  StackNode *discard = head;
  head = head->next;
  delete discard;
  stackSize--;
}

int Stack::top() const
{
  //Or throw an exception
  if(stackSize == 0 || head == nullptr)
  {
    std::cout << "Stack is empty.\n";
    exit(1);
  }
  return head->data;
}

int Stack::getMinimum() const
{
  //Or throw an exception
  if(stackSize == 0 || head == nullptr)
  {
    std::cout << "Stack is empty.\n";
    exit(1);
  }
  return (head->minimum->data);
}

bool Stack::isEmpty() const
{
  return (stackSize == 0 || head == nullptr);
}

int Stack::getSize() const
{
  return stackSize;
}


/*===================================================*/
/*  ./Chapter-3-Stacks-and-Queues/3.2-Stack-Min/StackNode.cpp  */
#include "StackNode.h"

StackNode::StackNode(int data, StackNode *next)
{
  this->data = data;
  this->next = next;
  minimum = nullptr;
}
/*===================================================*/
/*  ./Chapter-3-Stacks-and-Queues/3.2-Stack-Min/Stack.h  */
#ifndef STACK_H
#define STACK_H

#include "StackNode.h"
#include <iostream>
#include <cstdlib>

class Stack
{
public:
  Stack();
  virtual ~Stack();
  void push(int item);
  void pop();
  int top() const;
  int getMinimum() const;

  bool isEmpty() const;
  int getSize() const;
  
private:
  StackNode *head;
  int stackSize;
};

#endif // STACK_H
/*===================================================*/
/*  ./Chapter-3-Stacks-and-Queues/3.2-Stack-Min/StackNode.h  */
#ifndef STACKNODE_H
#define STACKNODE_H

class Stack;

class StackNode
{
public:
  friend class Stack;
  StackNode(int data, StackNode *next);

private:
  int data;
  StackNode *next;
  StackNode *minimum;
};

#endif // STACKNODE_H
/*===================================================*/
/*  ./Ch 4. Trees and Graphs/4.6_Successor.cpp  */
TreeNode* mostLeft(TreeNode* root) {
	while (root->left != nullptr) {
		root = root->left;
	}
	return root;
}

TreeNode* findSuccessor(TreeNode* root) {
	if (root->right != nullptr) {
		return mostLeft(root->right);
	}
	TreeNode* child = root;
	TreeNode* ancestor = root->parent;
	while(ancestor != nullptr && ancestor->right == child) {
		child = ancestor;
		ancestor = child->parent;
	}
	return ancestor;
}
/*===================================================*/
/*  ./Ch 4. Trees and Graphs/4.2_Minimal_Tree.cpp  */
TreeNode* createMinBST(vector<int> arr) {
	return createMinBST(arr, 0, arr.length() - 1);
}

TreeNode* createMinBST(vector<int> arr, int start, int end) {
	if (start > end) {
		return null;
	}
	int mid = start + (end - start) / 2;
	TreeNode* newNode = new TreeNode(arr[mid]);
	newNode.left = createMinBST(arr, start, mid - 1);
	newNode.right = createMinBST(arr, mid + 1, end);
	return newNode;
}
/*===================================================*/
/*  ./Ch 4. Trees and Graphs/4-1-Route-Between-Nodes.cpp  */
#include <vector>
#include <iostream>
#include <queue>
using namespace std;

// Graph Class implementing a directed graph in adjacency list representation
class Graph
{
public:
	int V;	// number of vertices
	vector<int> *adj;  //adjacency list 

	Graph(int V);
	void addEdge(int x, int y);
	bool isRoute(int x, int y);
};

// Constructor
Graph::Graph(int V)
{
	this->V=V;
	this->adj = new vector<int>[V];
}

// add a directed edge from x to y
void Graph::addEdge(int x, int y){
	adj[x].push_back(y);
}

// isRoute function to determine if there is a route from x to y
bool Graph::isRoute(int x, int y){

	vector<bool> visited(V,false);
	queue<int> q;

	q.push(x);

	while(!q.empty()){
		int curr = q.front();
		if (curr == y)
		{
			return true;
		}
		q.pop();
		visited[curr]= true;
		int n_size =adj[curr].size();
		for (int i = 0; i < n_size; ++i)
		{	
			if (!visited[adj[curr][i]])
			{
				q.push(adj[curr][i]);
			}
		}
	}
	return false;
}

int main(){
	
    Graph g(6);	
    g.addEdge(5, 2);
    g.addEdge(5, 0);
    g.addEdge(4, 0);
    g.addEdge(4, 1);
    g.addEdge(2, 3);
    g.addEdge(3, 1);

/*	
	Test graph :

	0 <---4---->1
	^ 			^
	|			|
	|			|
    5---->2---->3 

*/
    cout<<g.isRoute(5,4)<<endl; // false
    cout<<g.isRoute(5,1)<<endl; // true

    return 0;
}
/*===================================================*/
/*  ./Ch 4. Trees and Graphs/4.4_Check_Balanced.cpp  */
int checkHeight(TreeNode* root) {
	if (root == nullptr) {
		return 0;
	}
	int leftHeight = checkHeight(root->left);
	if (leftHeight == -1) {
		return -1;
	}
	int rightHeight = checkHeight(root->right);
	if (rightHeight == -1) {
		return -1;
	}

	if (abs(leftHeight - rightHeight) > 1) {
		return -1;
	}
	else {
		return max(leftHeight, rightHeight) + 1;
	}
}

bool isBalanced(TreeNode* root) {
	return (checkHeight == -1);
}
/*===================================================*/
/*  ./Ch 4. Trees and Graphs/C++14/4.11m-RandomTree.cpp  */
// Random Node: You are implementing a binary search tree class from scratch, which, in addition
// to insert, find, and delete, has a method getRandomNode() which returns a random node
// from the tree. All nodes should be equally likely to be chosen. Design and implement an algorithm
// for getRandomNode, and explain how you would implement the rest of the methods.

// Another solution with custom node implementation for balanced tree.
// getRundomNode: O(log(N)) time
// To fill tree from sorted array: O(N) time and O(N) memory.

#include <memory>
#include <array>
#include <vector>
#include "tree.hpp"
#include "treetestutils.hpp"

template <typename T, bool>
class RandomNode;

template <typename T, bool NotUsed = false>
class RandomNode
{
public:
    using NodePtr = std::shared_ptr<RandomNode<T>>;

    RandomNode(const T &v): size(1)
    {
        value = v;
    }

    const NodePtr &getLeft() const
    {
        return childs[0];
    }

    const NodePtr &getRight() const
    {
        return childs[1];
    }

    const T &getValue() const
    {
        return value;
    }

private:
    void add(const T &v)
    {
        if (!childs[v > value])
            childs[v > value] = std::make_shared<RandomNode<T>>(v);
        else
            childs[v > value]->add(v);
        ++size;
    }

    const T &getRandom() const
    {
        return get(std::rand() % size + 1);
    }

    const T &get(size_t n) const
    {
        if (n == size)
            return value;
        else if (childs[0] && n <= childs[0]->size)
            return childs[0]->get(n);
        else
            return childs[1]->get(n - (childs[0] ? childs[0]->size : 0));
    }

    T value;
    size_t size;
    std::array<NodePtr, 2> childs;

    template <typename U>
    friend class RandomTree;
};

template <typename T>
class RandomTree : public Tree<T, false, RandomNode>
{
    using Base = Tree<T, false, RandomNode>;

public:
    const T &getRandom() const
    {
        if (!Base::root)
            throw typename Base::TreeIsEmptyException();
        return Base::root->getRandom();
    }

    void add(const T &value)
    {
        if (Base::root)
            Base::root->add(value);
        else
            Base::root = std::make_shared<RandomNode<T, false>>(value);
    }
};


int main()
{
    std::srand (unsigned(std::time(0)));
    std::vector<int> v(7) ;
    std::iota(std::begin(v), std::end(v), 0); // Fill with 0, 1, ..., nodeCount - 1.
    std::random_shuffle(std::begin(v), std::end(v), [](int i){return std::rand() % i;});

    RandomTree<int> tree;
    tree.add(v.back());
    v.pop_back();
    for (auto i : v)
        tree.add(i);

    TestUtils::printTree(tree);
    std::cout << std::endl;

    // Check distribution of rundom tree nodes
    v = std::vector<int>(v.size() + 1, 0);
    for (auto i = 0U; i < v.size() * 1000; ++i)
        ++v[tree.getRandom()];

    size_t cnt = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "Total: " << cnt << " times\n";
    std::cout << std::fixed << std::setprecision(2);
    for (auto i = 0U; i < v.size(); ++i)
        std::cout << i << ": " << 100.0 * v[i] / cnt << "%\n";
}
/*===================================================*/
/*  ./Ch 4. Trees and Graphs/C++14/4.10i-CheckSubtree.cpp  */
// Check Subtree: Tl and T2 are two very large binary trees, with Tl much bigger than T2. Create an
// algorithm to determine if T2 is a subtree of Tl.
// A tree T2 is a subtree of Tl if there exists a node n in Tl such that the subtree of n is identical to T2.
// That is, if you cut off the tree at node n, the two trees would be identical.

// Solution with the Tree Node Iterator

#include "tree.hpp"
#include "treenodeiterator.hpp"
#include "treetestutils.hpp"

template <typename T>
bool compareTrees(const NodePtr<T> &left, const NodePtr<T> &right);

template <typename T>
bool checkSubtree(const Tree<T> &tree, const Tree<T> &subtree)
{
    auto subtreeRoot = subtree.getRoot();
    if (!subtreeRoot)
        return true; // empty subtree
    auto subtreeRootValue = subtreeRoot->getValue();
    
    for (const auto &node : tree)
    {
        if (node->getValue() == subtreeRootValue)
            if (compareTrees<int>(node, subtreeRoot))
                return true;
    }
    return false;
}

template <typename T>
bool compareTrees(const NodePtr<T> &left, const NodePtr<T> &right)
{
    if (!left && !right)
        return true;
    else if (!left || !right) // one tree is not finished, another already finished
        return false;
    else if (left->getValue() == right->getValue())
        return compareTrees<T>(left->getLeft(), right->getLeft()) && compareTrees<T>(left->getRight(), right->getRight());
    return false;
}

int main()
{
    auto left = TestUtils::getSampleTree<int>(15);
    auto right = TestUtils::getSampleTree<int>(7);
    TestUtils::printTree(right);
    std::cout << (checkSubtree(left, right) ? "is subtree of\n" : "is not subtree of\n");
    TestUtils::printTree(left);

    right = TestUtils::getSampleTree<int>(8);
    TestUtils::printTree(right);
    std::cout << (checkSubtree(left, right) ? "is subtree of\n" : "is not subtree of\n");
    TestUtils::printTree(left);
}/*===================================================*/
/*  ./Ch 4. Trees and Graphs/C++14/4.8p-FirstCommonAncestor.cpp  */
// First Common Ancestor: Design an algorithm and write code to find the first common ancestor
// of two nodes in a binary tree. Avoid storing additional nodes in a data structure. NOTE: This is not
// necessarily a binary search tree.
// New condition: tree nodes contain links to their parents.

#include "tree.hpp"
#include "treetestutils.hpp"
#include "treenodeiterator.hpp"

template <typename T>
int getDepth(const Tree<T, true> &tree, NodePtr<T, true> node)
{
    size_t depth = 0;
    if (node)
    {
        while (node->getParent())
        {
            node = node->getParent();
            ++depth;
        }
    }
    // Check if the node is in this tree
    if (node != tree.getRoot())
        depth =  -1;
    return depth;
}

template <typename T>
NodePtr<T, true> findCommonAncestor(const Tree<T, true> &tree, NodePtr<T, true> one, NodePtr<T, true> two)
{
    if (one == two)
        return one;

    auto depthL = getDepth(tree, one);
    auto depthR = getDepth(tree, two);
    if (depthL == -1 || depthR == -1)
        return nullptr;

    if (depthL > depthR)
    {
        std::swap(depthL, depthR);
        std::swap(one, two); // that is why we pass 'one' and 'two' arguments by values
    }

    while (depthR != depthL)
    {
        two = two->getParent();
        --depthR;
    }
    while (one != two)
    {
        one = one->getParent();
        two = two->getParent();
    }
    return one;
}

int main()
{
    auto tree = TestUtils::treeFromArray<int, true>({10, 1, 12, 3, 14, 25, 16, 27, 18, 29, 10, 13, 2, 15, 4, 5, 17, 7, 19, 9});
    TestUtils::printTree(tree);

    for (auto one : tree)
    {
        if (one == tree.getRoot())
            continue; // it is not interesting
        for (auto two : tree)
        {
            if (two == tree.getRoot() || two == one)
                continue; // it is not interesting
            auto ancestor = findCommonAncestor<int>(tree, one, two);
            std::cout << one->getValue() << ", " << two->getValue() << " <-- ";
            std::cout << (ancestor ? std::to_string(ancestor->getValue()) : "NOT FOUND") << std::endl;
        }
    }
}
/*===================================================*/
/*  ./Ch 4. Trees and Graphs/C++14/4.3-ListOfDepths.cpp  */
// List of Depths: Given a binary tree, design an algorithm which creates a linked list of all the nodes
// at each depth (e.g., if you have a tree with depth D, you'll have D linked lists).

// This task is mostly done in the function printTree :-)

#include <list>
#include "tree.hpp"
#include "treetestutils.hpp"

template <typename T>
using ListOfDepths = std::list<std::list<NodePtr<T>>>;

template <typename T>
ListOfDepths<T> getListOfDepths(const Tree<T> &tree)
{
    ListOfDepths<T> result;

    result.emplace_back();
    auto list = &result.back();
    list->push_back(tree.getRoot());

    do
    {
        result.emplace_back();
        auto &childs = result.back();
        for (const auto &n : *list)
        {
            if (n->getLeft())
                childs.push_back(n->getLeft());
            if (n->getRight())
                childs.push_back(n->getRight());
        }
        if (childs.empty())
        {
            result.pop_back();
            break;
        }
        list = &childs;
    } while (true);

    return result;
}

template <typename T>
void printDepths(const ListOfDepths<T> &depths)
{
    std::cout << "Nodes:\n";
    int depth = 0;
    for (const auto &line : depths)
    {
        std::cout << depth << ": ";
        auto sep = "";
        for (const auto &n : line)
        {
            std::cout << sep << n->getValue();
            sep = ", ";
        }
        std::cout << std::endl;
        ++depth;
    }
    std::cout << std::endl;
}

int main()
{
    auto tree = TestUtils::getSampleTree<int>(15);
    TestUtils::printTree(tree);
    auto depths = getListOfDepths(tree);
    printDepths<int>(depths);

    tree = TestUtils::getSampleTree<int>(20);
    TestUtils::printTree(tree);
    depths = getListOfDepths(tree);
    printDepths<int>(depths);
    return 0;
}
/*===================================================*/
/*  ./Ch 4. Trees and Graphs/C++14/4.5-ValidateBST.cpp  */
// Validate BST: Implement a function to check if a binary tree is a binary search tree.

#include <vector>
#include "tree.hpp"
#include "treetestutils.hpp"

template <typename T>
bool checkNode(const NodePtr<T> &node, const T *minValue, const T *maxValue)
{
    if (!node)
        return true;
    if (minValue && node->getValue() <= *minValue)
        return false;
    if (maxValue && node->getValue() > *maxValue)
        return false;

    return checkNode<T>(node->getLeft(), minValue, &node->getValue()) && checkNode<T>(node->getRight(), &node->getValue(), maxValue);
}

template <typename T>
bool isValidBST(const Tree<T> &tree)
{
    return checkNode<T>(tree.getRoot(), nullptr, nullptr);
}

int main()
{
    // valid BST
    auto tree = TestUtils::getSampleTree<int>(20);
    TestUtils::printTree(tree);
    std::cout << "The tree is " << (isValidBST<int>(tree) ? "" : "NOT ") << "binary search tree" << std::endl;

    // invalid BST
    tree.getRoot()->getRight()->getRight()->getRight()->setLeftChild(std::make_shared<Node<int>>(19));
    TestUtils::printTree(tree);
    std::cout << "The tree is " << (isValidBST<int>(tree) ? "" : "NOT ") << "binary search tree" << std::endl;

    // invalid BST
    tree.getRoot()->getRight()->getRight()->getRight()->setLeftChild(std::make_shared<Node<int>>(15));
    TestUtils::printTree(tree);
    std::cout << "The tree is " << (isValidBST<int>(tree) ? "" : "NOT ") << "binary search tree" << std::endl;
}
/*===================================================*/
/*  ./Ch 4. Trees and Graphs/C++14/4.10r-CheckSubtree.cpp  */
// Check Subtree: Tl and T2 are two very large binary trees, with Tl much bigger than T2. Create an
// algorithm to determine if T2 is a subtree of Tl.
// A tree T2 is a subtree of Tl if there exists a node n in Tl such that the subtree of n is identical to T2.
// That is, if you cut off the tree at node n, the two trees would be identical.

// Recursive solution with Tree traversing

#include "tree.hpp"
#include "treenodeiterator.hpp"
#include "treetestutils.hpp"

template <typename T>
bool compareTrees(const NodePtr<T> &left, const NodePtr<T> &right);

template <typename T>
bool checkSubtree(const NodePtr<T> &root, const NodePtr<T> &subtreeRoot);

template <typename T>
bool checkSubtree(const Tree<T> &tree, const Tree<T> &subtree)
{
    if (!subtree.getRoot())
        return true; // empty subtree
    return checkSubtree<T>(tree.getRoot(), subtree.getRoot());
}

template <typename T>
bool compareTrees(const NodePtr<T> &left, const NodePtr<T> &right)
{
    if (!left && !right)
        return true;
    else if (!left || !right) // one tree is not finished, another already finished
        return false;
    else if (left->getValue() == right->getValue())
        return compareTrees<T>(left->getLeft(), right->getLeft()) && compareTrees<T>(left->getRight(), right->getRight());
    return false;
}

template <typename T>
bool checkSubtree(const NodePtr<T> &root, const NodePtr<T> &subtreeRoot)
{
    if (!root)
        return false;
    if (root->getValue() == subtreeRoot->getValue() && compareTrees<T>(root, subtreeRoot))
        return true;
    return checkSubtree<T>(root->getLeft(), subtreeRoot) || checkSubtree<T>(root->getRight(), subtreeRoot);
}

int main()
{
    auto left = TestUtils::getSampleTree<int>(15);
    auto right = TestUtils::getSampleTree<int>(7);
    TestUtils::printTree(right);
    std::cout << (checkSubtree<int>(left, right) ? "is subtree of\n" : "is not subtree of\n");
    TestUtils::printTree(left);

    std::cout << std::endl;
    right = TestUtils::getSampleTree<int>(8);
    TestUtils::printTree(right);
    std::cout << (checkSubtree<int>(left, right) ? "is subtree of\n" : "is not subtree of\n");
    TestUtils::printTree(left);
}/*===================================================*/
/*  ./Ch 4. Trees and Graphs/C++14/4.7d-BuildOrder.cpp  */
// Build Order: You are given a list of projects and a list of dependencies (which is a list of pairs of
// projects, where the second project is dependent on the first project). All of a project's dependencies
// must be built before the project is. Find a build order that will allow the projects to be built. If there
// is no valid build order, return an error.
// EXAMPLE
// Input:
// projects: a, b, c, d, e, f
// dependencies: (a, d), (f, b), (b, d), (f, a), (d, c)
// Output: f, e, a, b, d, c

#include <list>
#include <iostream>

#include "graph.hpp"
#include "graphtestutils.hpp"

enum class States
{
    NotVisited,
    Visiting,
    Visited
};

bool buildProject(const Node<States> &node, std::list<Node<States>> &order)
{
    if (node->state == States::Visited)
        return true; // Already pushed in build order

    if (node->state == States::Visiting)
    {
        // Dependency cycle detected
        order.empty();
        return false;
    }

    node->state = States::Visiting;
    for (auto &a : node->getAdjacent())
    {
        auto prj = a.lock();
        if (prj)
            if (!buildProject(prj, order))
                return false;
    }
    node->state = States::Visited;
    order.push_front(node);
    return true;
}

std::list<Node<States>> buildOrder(const Graph<States> &graph)
{
    std::list<Node<States>> order;
    auto &projects = graph.getNodes();
    for (auto &p : projects)
        if (!buildProject(p, order))
            break;
    return order;
}

void test(const Graph<States> &graph)
{
    auto order = buildOrder(graph);

    const char *sep = "";
    for (auto &n : order)
    {
        std::cout << sep << n->Name();
        sep = ", ";
    }
    std::cout << std::endl;
}

int main()
{
    test(TestUtils::createGraph<States>({"a", "b", "c", "d", "e", "f"},
        {{"a", "d"}, {"f", "b"}, {"b", "d"}, {"f", "a"}, {"d", "c"}}));
    
    test(TestUtils::createGraph<States>({"a", "b", "c", "d", "e", "f", "g"},
        {{"a", "e"}, {"b", "a"}, {"b", "e"}, {"c", "a"}, {"d", "g"}, {"f", "a"}, {"f", "b"}, {"f", "c"}}));
}
/*===================================================*/
/*  ./Ch 4. Trees and Graphs/C++14/4.6-Successor.cpp  */
// Successor: Write an algorithm to find the "next" node (i.e., in-order successor) of a given node in a
// binary search tree. You may assume that each node has a link to its parent.

#include <vector>
#include "tree.hpp"
#include "treetestutils.hpp"

template <typename T>
NodePtr<T, true> getMin(NodePtr<T, true> node)
{
    if (!node)
        return nullptr;
    while (node->getLeft())
        node = node->getLeft();
    return node;
}

template <typename T>
NodePtr<T, true> nextNode(NodePtr<T, true> node)
{
    if (!node)
        return nullptr;
    if (node->getRight())
        return getMin<T>(node->getRight());
    auto next = node->getParent();
    while (next && node == next->getRight())
    {
        // right subtree is traversed -> parent and all its childs were traversed
        node = next;
        next = next->getParent();
    }
    return next;
}

int main()
{
    // valid BST
    auto tree = TestUtils::getSampleTree<int, true>(20);
    TestUtils::printTree(tree);
    auto node = getMin<int>(tree.getRoot());
    if (node)
    {
        std::cout << node->getValue();
        for (node = nextNode<int>(node); node; node = nextNode<int>(node))
            std::cout << " --> " << node->getValue();
        std::cout << std::endl;
    }
}
/*===================================================*/
/*  ./Ch 4. Trees and Graphs/C++14/4.2-MinimalTree.cpp  */
// Minimal Tree: Given a sorted (increasing order) array with unique integer elements, write an
// algorithm to create a binary search tree with minimal height.

#include <array>
#include <numeric>
#include "tree.hpp"
#include "treetestutils.hpp"

template <typename T>
NodePtr<T> subtreeFromArray(const T *array, int start, int end)
{
    if (end < start)
        return nullptr;

    int i = (start + end) / 2;
    auto node = std::make_shared<Node<T>>(array[i]);
    node->setLeftChild(subtreeFromArray(array, start, i - 1));
    node->setRightChild(subtreeFromArray(array, i + 1, end));
    return node;
}

template <typename T>
Tree<T> treeFromArray(const T *array, size_t size)
{
    Tree<T> tree;
    tree.setRoot(subtreeFromArray(array, 0, size - 1));

    return tree;
}

int main()
{
    std::array<int, 63> array;
    std::iota(array.begin(), array.end(), 0); // Fill with 0 .. 62

    for (auto &i : {1, 2, 3, 6, 7, 8, 14, 15, 16, 29, 30, 31})
    {
        auto tree = treeFromArray(&array[0], i);
        TestUtils::printTree(tree);
    }
    return 0;
}
/*===================================================*/
/*  ./Ch 4. Trees and Graphs/C++14/4.4-CheckBalanced.cpp  */
// Check Balanced: Implement a function to check if a binary tree is balanced. For the purposes of
// this question, a balanced tree is defined to be a tree such that the heights of the two subtrees of any
// node never differ by more than one.

#include <limits>
#include "tree.hpp"
#include "treetestutils.hpp"

template <typename T>
int getHeight(const NodePtr<int> &node)
{
    if (!node)
        return 0;

    int leftH = getHeight<T>(node->getLeft());
    if (leftH == -1)
        return -1;

    int rightH = getHeight<T>(node->getRight());
    if (rightH == -1)
        return -1;
    
    if (std::abs(leftH - rightH) > 1)
        return -1;
    return std::max(leftH, rightH) + 1;
}

template <typename T>
bool isTreeBalanced(const Tree<T> &tree)
{
    return getHeight<T>(tree.getRoot()) != -1;
}

int main()
{
    auto tree = TestUtils::getSampleTree<int>(20); // balanced tree
    std::cout << "Tree is " << (isTreeBalanced<int>(tree) ? "" : "NOT ") << "balanced" << std::endl;

    // Disconnect right subtree, the result tree is not balanced
    auto r = tree.getRoot()->getRight();
    tree.getRoot()->getRight() = std::make_shared<Node<int>>(100);
    std::cout << "Tree is " << (isTreeBalanced<int>(tree) ? "" : "NOT ") << "balanced" << std::endl;

    // Return right subtree and check if the tree is balanced again
    tree.getRoot()->getRight() = r; // balanced
    std::cout << "Tree is " << (isTreeBalanced<int>(tree) ? "" : "NOT ") << "balanced" << std::endl;
    return 0;
}
/*===================================================*/
/*  ./Ch 4. Trees and Graphs/C++14/4.12-PathsWithSum.cpp  */
// Paths with Sum: You are given a binary tree in which each node contains an integer value (which
// might be positive or negative). Design an algorithm to count the number of paths that sum to a
// given value. The path does not need to start or end at the root or a leaf, but it must go downwards
// (traveling only from parent nodes to child nodes).

#include <unordered_map>

#include "tree.hpp"
#include "treetestutils.hpp"

int sumsFrom(const NodePtr<int> &node, int requiredSum, int pathSum, std::unordered_map<int, int> &sums)
{
    if (!node)
        return 0;

    pathSum += node->getValue();

    // Count of paths ending here and having required sum
    int overflow = pathSum - requiredSum;
    int cnt = sums.count(overflow) ? sums.at(overflow) : 0;

    // Starting from root
    if (pathSum == requiredSum)
        ++cnt;

    sums[pathSum] += 1;
    cnt += sumsFrom(node->getLeft(), requiredSum, pathSum, sums);
    cnt += sumsFrom(node->getRight(), requiredSum, pathSum, sums);

    // Done with this node, do not use pathSum till this node anymore
    if ((sums[pathSum] -= 1) == 0)
        sums.erase(pathSum); // less memory
    return cnt;
}

int countPathsWithSum(const Tree<int> &tree, int sum)
{
        std::unordered_map<int, int> tmp;
        return sumsFrom(tree.getRoot(), sum, 0, tmp);
}

int main()
{
    auto tree = TestUtils::treeFromArray({1, -2, 3, -5, 7, -11, 13, -1, 2, -3, 5, -7, 11, -1, 2, -3, 1, -2, 3, -7});
    TestUtils::printTree(tree);

    // From sum of negative values till sum of positive values
    for (int i = -42; i <= 48; ++i)
        std::cout << "Sum " << i << " can be reached in " << countPathsWithSum(tree, i) << " paths\n";
}
/*===================================================*/
/*  ./Ch 4. Trees and Graphs/C++14/4.11-RandomNode.cpp  */
// Random Node: You are implementing a binary search tree class from scratch, which, in addition
// to insert, find, and delete, has a method getRandomNode() which returns a random node
// from the tree. All nodes should be equally likely to be chosen. Design and implement an algorithm
// for getRandomNode, and explain how you would implement the rest of the methods.

// Tree contains all nodes in a deque. It needs to handle insert() and delete() methods.
// getRundomNode: O(1) time
// To fill tree with deque additional: + O(N) time + O(N) memory.
// To clean tree with deque additional: + O(N) time

#include <memory>
#include <deque>
#include <random>
#include <chrono>
#include <iostream>

template <typename T>
class Tree
{
public:
    Tree()
    {
        // construct a trivial random generator engine from a time-based seed:
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        randomEngine = std::default_random_engine(seed);
    }

    class RandomNode;
    using NodePtr = std::shared_ptr<RandomNode>;

    class RandomNode
    {
    public:
        template <typename U>
        explicit RandomNode(U &&value) : v(std::forward<U>(value))
        {
        }

        const NodePtr &left() const
        {
            return childs[0];
        }
        const NodePtr &right() const
        {
            return childs[1];
        }
        const T &value() const
        {
            return v;
        }

    private:
        std::array<NodePtr, 2> childs;
        T v;
        size_t idx; // index in the Tree::nodes, used for quick deletion from Tree::nodes
        friend class Tree;
    };

    template <typename U>
    const NodePtr insert(const NodePtr &parent, U &&value, bool insertRight)
    {
        NodePtr newNode;
        if (!parent)
            rootNode = newNode = std::make_shared<RandomNode>(std::forward<U>(value));
        else
            parent->childs[insertRight ? 1 : 0] = newNode = std::make_shared<RandomNode>(std::forward<U>(value));
        newNode->idx = nodes.size();
        nodes.push_back(newNode);
        return newNode;
    }

    const NodePtr getRundomNode() const
    {
        if (nodes.size() == 0)
            return nullptr;
        else if (nodes.size() == 1)
            return rootNode;
        else
        {
            if (randomDistribution.max() != nodes.size() - 1)
                randomDistribution = std::uniform_int_distribution<size_t>(0, nodes.size() - 1);
            return nodes.at(randomDistribution(randomEngine));
        }
    }

    const NodePtr &root() const
    {
        return this->rootNode;
    }

private:
    NodePtr rootNode;
    std::deque<NodePtr> nodes;
    mutable std::default_random_engine randomEngine;
    mutable std::uniform_int_distribution<size_t> randomDistribution;
};

int main()
{
    Tree<int> tree;
    auto node = tree.insert(nullptr, 3, false); //      3
    auto left = tree.insert(node, 1, false);    //    /   \       .
    tree.insert(left, 0, false);                //   1     5
    tree.insert(left, 2, true);                 //  / \   / \     .
    auto right = tree.insert(node, 5, true);    // 0   2 4   6
    tree.insert(right, 4, false);
    tree.insert(right, 6, true);

    std::string coma;
    for (int i = 0; i < 100; ++i)
    {
        std::cout << coma << tree.getRundomNode()->value();
        if (coma.empty())
            coma = ", ";
    }
    std::cout << std::endl;
}
/*===================================================*/
/*  ./Ch 4. Trees and Graphs/C++14/4.9-BSTSequences.cpp  */
// BST Sequences: A binary search tree was created by traversing through an array from left to right
// and inserting each element. Given a binary search tree with distinct elements, print all possible
// arrays that could have led to this tree.
// EXAMPLE
// Input:   2
//        1   3
// Output: {2, 1, 3}, {2, 3, 1}

#include <deque>
#include <list>
#include <cassert>
#include "tree.hpp"
#include "treetestutils.hpp"

template <typename T>
std::list<std::deque<T>> possibleStableJoins(std::deque<T> &prefix, std::deque<T> &left, std::deque<T> &right);

template <typename T>
std::list<std::deque<T>> possibleArrays(const NodePtr<T> &root)
{
    std::list<std::deque<T>> result;
    if (!root)
    {
        result.emplace_back(); // empty is required to call cycle body at least once
        return result;
    }

    auto leftArrays = possibleArrays<T>(root->getLeft());
    auto rightArrays = possibleArrays<T>(root->getRight());

    std::deque<T> prefix;
    prefix.push_back(root->getValue());
    for (auto &left : leftArrays)
        for (auto &right : rightArrays)
        {
            auto joins = possibleStableJoins<T>(prefix, left, right);
            result.splice(result.end(), std::move(joins));
            assert(prefix.size() == 1 && prefix[0] == root->getValue());
        }
    return result;
}

template <typename T>
std::list<std::deque<T>> possibleStableJoins(std::deque<T> &prefix, std::deque<T> &left, std::deque<T> &right)
{
    std::list<std::deque<T>> result;

    if (left.empty() || right.empty())
    {
        std::deque<T> r(prefix);
        r.insert(r.end(), left.begin(), left.end());
        r.insert(r.end(), right.begin(), right.end());
        result.push_back(std::move(r));
        return result;
    }

    prefix.push_back(left.front());
    left.pop_front();
    result.splice(result.end(), possibleStableJoins(prefix, left, right));
    left.push_front(prefix.back());
    prefix.pop_back();

    prefix.push_back(right.front());
    right.pop_front();
    result.splice(result.end(), possibleStableJoins(prefix, left, right));
    right.push_front(prefix.back());
    prefix.pop_back();

    return result;
}

int main()
{
    auto tree = TestUtils::treeFromArray({5, 10, 15, 20, 25, 50, 60, 65, 70, 80});
    TestUtils::printTree(tree);
    // auto tree = TestUtils::treeFromArray({2, 3, 1});
    auto result = possibleArrays<int>(tree.getRoot());

    for (auto &array : result)
    {
        std::string sep;
        std::cout << "{";
        for (auto &n : array)
        {
            std::cout << sep << n;
            if (sep.empty())
                sep = ", ";
        }
        std::cout << "}" << std::endl;
    }
}
/*===================================================*/
/*  ./Ch 4. Trees and Graphs/C++14/4.1-RouteBetweenNodes.cpp  */
// Route Between Nodes: Given a directed graph, design an algorithm to find out whether is a
// route between to nodes.

#include <queue>
#include <iostream>
#include <cassert>
#include "graph.hpp"
#include "graphtestutils.hpp"

enum State {Unvisited, Visiting, Visited};

// Width visiting
bool routeBetwenNodes(const Graph<State> &graph, const Node<State> &from, const Node<State> &to)
{
    if (from == to)
        return true;

    for (auto &n : graph.getNodes())
        n->state = Unvisited;

    std::queue<Node<State>> queue;

    from->state = Visiting;
    queue.push(from);

    while (!queue.empty())
    {
        Node<State> n = queue.front();
        queue.pop();
        for (auto &c : n->getAdjacent())
        {
            auto v = c.lock();
            if (v && v->state == Unvisited)
            {
                if (v == to)
                    return true;
                else
                {
                    v->state = Visiting;
                    queue.push(v);
                }
            }
        }
        n->state = Visited;
    }
    return false;
}

// Recursive visiting

bool routeBetwenNodesWalker(const Graph<State> &graph, const Node<State> &from, const Node<State> &to)
{
    if (from == to)
        return true;

    from->state = Visited;

    for (auto &c : from->getAdjacent())
    {
        auto node = c.lock();
        if (node && node->state != Visited)
        {
            if (routeBetwenNodesWalker(graph, node, to))
                return true;
        }
    }
    return false;
}

bool routeBetwenNodesR(const Graph<State> &graph, const Node<State> &from, const Node<State> &to)
{
    for (auto &n : graph.getNodes())
        n->state = Unvisited;

    return routeBetwenNodesWalker(graph, from, to);
}

bool test(const Graph<State> &graph, size_t from, size_t to)
{
    auto &fromNode = graph[from], &toNode = graph[to];
    bool result = routeBetwenNodes(graph, fromNode, toNode);
    bool resultR = routeBetwenNodesR(graph, fromNode, toNode);

    assert(result == resultR);
    std::cout << toNode->Name() << " is " << (result ? "" : "NOT ") << "reachable from " << fromNode->Name() << std::endl;
    std::cout << toNode->Name() << " is " << (resultR ? "" : "NOT ") << "reachable from " << fromNode->Name() << std::endl;
    return result;
}

void testGraph(const Graph<State> &graph)
{
    auto size = graph.getNodes().size();
    for (decltype(size) i = 0; i < size; ++i)
    {
        for (decltype(size) j = 0; j < size; ++j)
            test(graph, i, j);
        std::cout << std::endl;
    }

}

int main()
{
    testGraph(TestUtils::getExampleGraph<State>());
    testGraph(TestUtils::getExampleGraph2<State>());
    testGraph(TestUtils::getExampleGraph3<State>());
    return 0;
}
/*===================================================*/
/*  ./Ch 4. Trees and Graphs/C++14/4.8-FirstCommonAncestor.cpp  */
// First Common Ancestor: Design an algorithm and write code to find the first common ancestor
// of two nodes in a binary tree. Avoid storing additional nodes in a data structure. NOTE: This is not
// necessarily a binary search tree.

// My solution is faster than one from the book. commonAncestor() or commonAncHelper() function from
// the book always recursively traverse subtrees till leafs, but my solutions does it twice rarer.
// In O-notation speeds are the same.

#include <vector>
#include "tree.hpp"
#include "treetestutils.hpp"
#include "treenodeiterator.hpp"

enum FindResult
{
    NotFound,   // node not found in subtree
    FoundLeft,  // node found in left subtree
    FoundRight, // node found in right subtree
    FoundEqual  // node is subtree root
};

template <typename T>
FindResult findNodeFrom(const NodePtr<T> &startNode, const NodePtr<T> &node)
{
    if (!startNode)
        return NotFound;
    if (startNode == node)
        return FoundEqual;
    if (findNodeFrom<T>(startNode->getLeft(), node) != NotFound)
        return FoundLeft;
    else if (findNodeFrom<T>(startNode->getRight(), node) != NotFound)
        return FoundRight;
    return NotFound;
}

template <typename T>
NodePtr<T> findCommonAncestor(const Tree<T> &tree, const NodePtr<T> &one, const NodePtr<T> &two)
{
    if (one == two)
        return one;
    
    auto startNode = tree.getRoot();

    auto firstResult = findNodeFrom<T>(startNode, one);
    if (firstResult == NotFound)
        return nullptr;
    auto secondResult = findNodeFrom<T>(startNode, two);
    if (secondResult == NotFound)
        return nullptr;

    while (firstResult == secondResult)
    {
        startNode = (firstResult == FoundLeft) ? startNode->getLeft() : startNode->getRight();
        firstResult = findNodeFrom<T>(startNode, one);
        secondResult = findNodeFrom<T>(startNode, two);
    }
    return startNode;
}

int main()
{
    auto tree = TestUtils::treeFromArray({10, 1, 12, 3, 14, 25, 16, 27, 18, 29, 10, 13, 2, 15, 4, 5, 17, 7, 19, 9});
    TestUtils::printTree(tree);

    for (auto one : tree)
    {
        if (one == tree.getRoot())
            continue; // it is not interesting
        for (auto two : tree)
        {
            if (two == tree.getRoot() || two == one)
                continue; // it is not interesting
            auto ancestor = findCommonAncestor<int>(tree, one, two);
            std::cout << one->getValue() << ", " << two->getValue() << " <-- ";
            std::cout << (ancestor ? std::to_string(ancestor->getValue()) : "NOT FOUND") << std::endl;
        }
    }

    // Test nodes of different trees
    auto tree2 = TestUtils::getSampleTree<int>(7);
    auto node1 = tree.getRoot()->getLeft()->getRight();
    auto node2 = tree2.getRoot()->getRight()->getLeft();
    auto ancestor = findCommonAncestor<int>(tree, node1, node2);
    std::cout << "Nodes below are of diffent trees:\n";
    std::cout << node1->getValue() << ", " << node2->getValue() << " <-- ";
    std::cout << (ancestor ? std::to_string(ancestor->getValue()) : "NOT FOUND") << std::endl;
}
/*===================================================*/
/*  ./Ch 4. Trees and Graphs/C++14/4.7-BuildOrder.cpp  */
// Build Order: You are given a list of projects and a list of dependencies (which is a list of pairs of
// projects, where the second project is dependent on the first project). All of a project's dependencies
// must be built before the project is. Find a build order that will allow the projects to be built. If there
// is no valid build order, return an error.
// EXAMPLE
// Input:
// projects: a, b, c, d, e, f
// dependencies: (a, d), (f, b), (b, d), (f, a), (d, c)
// Output: f, e, a, b, d, c

#include <list>
#include <iostream>

#include "graph.hpp"
#include "graphtestutils.hpp"

std::list<Node<int>> buildOrder(const Graph<int> &graph)
{
    std::list<Node<int>> order;
    auto &projects = graph.getNodes();

    for (auto &n : projects)
    {
        for (auto &c : n->getAdjacent())
            c.lock()->state++;  // skip check c.lock()
    }
    for (auto &n : projects)
        if (n->state == 0)
            order.push_back(n);

    for (auto &p : order)
    {
        for (auto &c : p->getAdjacent())
        {
            auto n = c.lock();
            if (n && (--n->state) == 0)
                order.push_back(n);
        }
    }

    if (order.size() != projects.size())
        order.clear();
    return order;
}


void test(const Graph<int> &graph)
{
    auto order = buildOrder(graph);

    const char *sep = "";
    for (auto &n : order)
    {
        std::cout << sep << n->Name();
        sep = ", ";
    }
    std::cout << std::endl;
}

int main()
{
    test(TestUtils::createGraph<int>({"a", "b", "c", "d", "e", "f"},
        {{"a", "d"}, {"f", "b"}, {"b", "d"}, {"f", "a"}, {"d", "c"}}));

    test(TestUtils::createGraph<int>({"a", "b", "c", "d", "e", "f", "g"},
        {{"a", "e"}, {"b", "a"}, {"b", "e"}, {"c", "a"}, {"d", "g"}, {"f", "a"}, {"f", "b"}, {"f", "c"}}));
}
/*===================================================*/
/*  ./Ch 4. Trees and Graphs/4.10_Check_Subtree.cpp  */
bool containTree(TreeNode* T1, TreeNode* T2) {
	string s1 = "", s2 = "";
	inOrderString(T1, s1);
	inOrderString(T2, s2);
	return (s1.find(s2) != string::npos);
}

void inOrderString(TreeNode* root, string s) {
	if (root == nullptr) {
		s += "X";
		return;
	}
	s += root->data;
	inOrderString(root->left, s);
	inOrderString(root->right, s);
}
/*===================================================*/
/*  ./Ch 4. Trees and Graphs/4.5_Validate_BST.cpp  */
bool validateBST(TreeNode* root, int min, int max) {
	if (root == nullptr) {
		return true;
	}
	if (root->data < min || root->data > max) {
		return false;
	}
	return validateBST(root->left, min, root->data - 1) && validateBST(root->right, root->data + 1, max);
}

bool validateBST(TreeNode* root) {
	return validateBST(root, -2e9, 2e9);
}
/*===================================================*/
/*  ./Ch 4. Trees and Graphs/4-2-Minimal-Tree.cpp  */
#include<bits/stdc++.h>
using namespace std;

struct Node {
    int data;
    Node *left;
    Node *right;
    Node(int x) : data(x), left(NULL), right(NULL) {}
 };

void preOrder(Node *node)
{
	if (node == NULL)
		return;
	cout<< node->data <<" ";
	preOrder(node->left);
	preOrder(node->right);
}

Node* createMinimalBST(vector<int> arr, int low, int high)
{
    if (low > high) {
        return NULL;
    }
    int mid = (low + high) / 2;
    Node* node = new Node(arr[mid]);
    node->left = createMinimalBST(arr, low, mid - 1);
    node->right = createMinimalBST(arr, mid + 1, high);
    return node;
}

Node *createMinimalBST(vector<int> arr)
{
    if (arr.size() == 0)
        return NULL;
    return createMinimalBST( arr, 0, ((int)arr.size()) - 1 );
}

int main()
{
    int A[] = {1, 2, 3, 4, 5, 6, 7};

    vector<int> arr (A, A + sizeof(A) / sizeof(A[0]) );
    /*
        Convert List to BST {1,2,3,4,5,6,7}
                         4
                      /     \
                    2         6
                  /   \     /   \
                 1     3   5     7
    */
    Node *root = createMinimalBST(arr);
    cout<<"\nPreOrder Traversal of constructed BST : ";
    preOrder(root);

    return 0;
}
/*===================================================*/
/*  ./Ch 5. Bit Manipulation/C++14/5.2-BinaryToString.cpp  */
// Binary to String: Given a real number between 0 and 1 (e.g., 0.72) that is passed in as a double,
// print the binary representation. If the number cannot be represented accurately in binary with at
// most 32 characters, print "ERROR:'

#include <cstdint>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <limits>

std::string binaryToStringMul(double binary)
{
    std::stringstream str;
    if (binary < 0 || binary > 1)
        return "ERROR";
    if (binary == 1)
        return "1";
    if (binary == 0)
        return "0";

    str << "0.";

    while (binary != 0)
    {
        if (str.tellp() > 32)
            return "ERROR";

        binary *= 2;
        if (binary >= 1)
        {
            str << '1';
            binary -= 1;
        }
        else
            str << '0';
    }
    return str.str();
}

std::string binaryToStringDiv(double binary)
{
    std::stringstream str;
    if (binary < 0 || binary > 1)
        return "ERROR";
    if (binary == 1)
        return "1";
    if (binary == 0)
        return "0";

    double mantissaBit = 0.5;
    str << "0.";

    while (binary != 0)
    {
        if (str.tellp() > 32)
            return "ERROR";

        if (binary >= mantissaBit)
        {
            str << '1';
            binary -= mantissaBit;
        }
        else
            str << '0';
        mantissaBit /= 2;
    }
    return str.str();
}

int main()
{
    double value = 0;
    double frac = 1.0 / 2 + 1.0 / 8;

    std::cout << std::setprecision(std::numeric_limits<double>::digits10 + 1);

    for (int i = 0; i < 10; ++i)
    {
        std::cout << value << ": " << binaryToStringMul(value) << "(b), " << binaryToStringDiv(value) << "(b)\n";
        value += frac;
        frac /= 16;
    }
}
/*===================================================*/
/*  ./Ch 5. Bit Manipulation/C++14/5.1-Insertion.cpp  */
// Insertion: You are given two 32-bit numbers, N and M, and two bit positions, i and j. Write a method
// to insert M into N such that M starts at bit j and ends at bit i. You can assume that the bits j through
// i have enough space to fit all of M. That is, if M = 10011, you can assume that there are at least 5
// bits between j and i. You would not, for example, have j = 3 and i = 2, because M could not fully
// fit between bit 3 and bit 2.
// EXAMPLE
// Input:  N = 10000000000, M = 10011, i = 2, j = 6
// Output: N = 10001001100

#include <cstdint>
#include <iostream>

#include "bitutils.hpp"

int32_t insertion(int32_t N, int32_t M, int i, int j)
{
    int w = j - i;
    if (w <= 0)
        return N;

    int32_t maskN = (~0 << (j + 1)) | ((1 << i) - 1);
    return (N & maskN) | (M << i);
}

int main()
{
    auto result = insertion(0b10000000000, 0b10011, 2, 6);
    std::cout << bits<decltype(result), 11>(result);
}
/*===================================================*/
/*  ./Ch 5. Bit Manipulation/C++14/5.3-FlipBitToWin.cpp  */
// Flip Bit to Win: You have an integer and you can flip exactly one bit from a O to a 1. Write code to
// find the length of the longest sequence of 1 s you could create.
// EXAMPLE
// Input:  1775 (or: 11011101111)
// Output: 8

#include <cstdint>
#include <type_traits>
#include <algorithm>
#include <limits>
#include <iostream>

#include "bitutils.hpp"

template <typename T>
int flipToWin(T bits)
{
    static_assert(std::is_unsigned<T>::value, "T must be unsigned integral type");

    int result = 1;
    int lenLeft = 0;
    int lenRight = 0;
    while (bits != 0)
    {
        if (bits & 1)
            ++lenLeft;
        else
        {
            // The current bit is 0 means that we counted right sequence length.
            // If the next bit will be 0, then right sequence length will reset to 0.
            lenRight = lenLeft;
            lenLeft = 0;
        }
        bits >>= 1;
        result = std::max(lenRight + lenLeft + 1, result);
    }
    // We can not flip bit to 1 if there was no bit 0
    return std::min(result, std::numeric_limits<T>::digits);
}

int main()
{
    for (auto testValue : {
                0b11011101111U
            ,  0b110111001111U
            , 0b1100111001111U
            , 0U
            , ~0U
        })
        std::cout << bits(testValue) << ": " << flipToWin(testValue) << std::endl;
}
/*===================================================*/
/*  ./Ch 5. Bit Manipulation/C++14/5.4-NextNumber.cpp  */
// Next Number: Given a positive integer, print the next smallest and the previous largest number that
// have the same number of 1 bits in their binary representation.

#include <tuple>
#include <iostream>
#include "bitutils.hpp"

template <typename T>
std::tuple<T, T> nextBiggerSmaller(T v)
{
    static_assert(std::is_unsigned<T>::value, "T must be unsigned integral type");

    auto result = std::make_tuple(0U, 0U);
    T rightZeros = 0; // tail zero count
    T rightOnes = 0;  // count of tightmost ones between zeros
    T zeroPos = 0;    // first/rightmost zero of 01 bits
    T tmp = v;

    while (tmp != 0 && !(tmp & 1))
    {
        ++rightZeros;
        tmp >>= 1;
    }
    while (tmp != 0 && (tmp & 1))
    {
        ++rightOnes;
        tmp >>= 1;
    }
    zeroPos = rightZeros + rightOnes;
    if (0 < zeroPos && zeroPos < std::numeric_limits<decltype(v)>::digits)
    {
        tmp = v | (1 << zeroPos);     // rightmost non-trailing zero to 1
        // at this point we have one more bit 1
        tmp &= ~((1 << zeroPos) - 1); // reset all bits right from previously set
        tmp |= (1 << (rightOnes - 1)) - 1; // set rightOnes - 1 rightmost bits to 1
        std::get<1>(result) = tmp;
    }

    rightOnes = rightZeros = 0;
    tmp = v;

    while (tmp != 0 && (tmp & 1))
    {
        ++rightOnes;
        tmp >>= 1;
    }
    while (tmp != 0 && !(tmp & 1))
    {
        ++rightZeros;
        tmp >>= 1;
    }
    T onePos = rightZeros + rightOnes; // first/rightmost one of 10 bits
    if (0 < onePos && onePos < std::numeric_limits<decltype(v)>::digits)
    {
        tmp = v & ~(1 << onePos);  // rightmost non-trailing 1 to zero
        // at this point we have one bit 1 less
        tmp |= (1 << onePos) - 1;  // set all bits right from previously reset to 0
        tmp &= ~((1 << (rightZeros - 1)) - 1); // reset rightZeros - 1 rightmost bits to 0
        std::get<0>(result) = tmp;
    }

    return result;
}

int main()
{
    for (uint16_t v : {
              0b11011101111U
            , 0U
            , 0b1111111111111111U
            , 0b0111111111111111U
            , 0b1111111111111110U})
    {
        auto minMax = nextBiggerSmaller(v);
        auto nextMin = std::get<1>(minMax);
        auto prevMax = std::get<0>(minMax);
        std::cout << v << " (" << bits(v) << "b " << countBits(v) << " bits)\nNext min: ";
        if (nextMin > 0)
            std::cout << nextMin << " (" << bits(nextMin) << "b " << countBits(static_cast<unsigned>(nextMin)) << " bits)";
        else
            std::cout << "Not existing";
        std::cout << "\nPrev max: ";
        if (prevMax > 0)
            std::cout << prevMax << " (" << bits(prevMax) << "b " << countBits(static_cast<unsigned>(prevMax)) << " bits)";
        else
            std::cout << "Not existing";
        std::cout << std::endl << std::endl;
    }

    uint8_t v = 1; // rightmost
    do
    {
        std::cout << bits(v) << '\n';
        v = std::get<1>(nextBiggerSmaller(v));
    }
    while (v != 0);
    std::cout << std::endl;
    v = 1 << (std::numeric_limits<decltype(v)>::digits - 1);
    do
    {
        std::cout << bits(v) <<  '\n';
        v = std::get<0>(nextBiggerSmaller(v));
    }
    while (v != 0);
}
/*===================================================*/
/*  ./Ch 6. Math and Logic Puzzles/7.The Apocalypse/The Apocalypse.cpp  */
#include <string>
#include <vector>
#include <iostream>

void std::vector<int> runOneFamily()
{
		Random *random = new Random();
		int boys = 0;
		int girls = 0;
		while (girls == 0)
		{ 
			if (random->nextBoolean())
			{ 
				girls += 1;
			}
			else
			{ 
				boys += 1;
			}
		}
		std::vector<int> genders = {girls, boys};
		return genders;
}

	double runNFamilies(int n)
	{
		int boys = 0;
		int girls = 0;
		for (int i = 0; i < n; i++)
		{
			std::vector<int> genders = runOneFamily();
			girls += genders[0];
			boys += genders[1];
		}
		return girls / static_cast<double>(boys + girls);
	}

	static void main(std::vector<std::wstring> &args)
	{
		double ratio = runNFamilies(10000000);
		std::wcout << ratio << std::endl;

	}

}
/*===================================================*/
/*  ./Ch 6. Math and Logic Puzzles/Intro/SieveOfEratosthenes.cpp  */
#include <string>
#include <vector>
#include <iostream>
#include <cmath>

void crossOff(std::vector<bool> &flags, int prime)
{		 	
		for (int i = prime * prime; i < flags.size(); i += prime)
		{
			flags[i] = false;
		}
}

	int getNextPrime(std::vector<bool> &flags, int prime)
	{
		int next = prime + 1;
		while (next < flags.size() && !flags[next])
		{
			next++;
		}
		return next;
	}

	void init(std::vector<bool> &flags)
	{
		flags[0] = false;
		flags[1] = false;
		for (int i = 2; i < flags.size(); i++)
		{
			flags[i] = true;
		}
	}

	void std::vector<int> prune(std::vector<bool> &flags, int count)
	{
		std::vector<int> primes(count);
		int index = 0;
		for (int i = 0; i < flags.size(); i++)
		{
			if (flags[i])
			{
				primes[index] = i;
				index++;
			}
		}
		return primes;
	}

	void std::vector<bool> sieveOfEratosthenes(int max)
	{
		std::vector<bool> flags(max + 1);

		init(flags);
		int prime = 2;

		while (prime <= sqrt(max))
		{
			   
			crossOff(flags, prime);

			
			prime = getNextPrime(flags, prime);
		}

		return flags;
	}

	void main(std::vector<std::wstring> &args)
	{
		std::vector<bool> primes = sieveOfEratosthenes(4);
		for (int i = 0; i < primes.size(); i++)
		{
			if (primes[i])
			{
				std::wcout << i << std::endl;
			}
		}
	}

}
/*===================================================*/
/*  ./Ch 6. Math and Logic Puzzles/Intro/Prime.cpp  */
#include <iostream>

using namespace std;

int main()
{
  int i,j,sq;
  int min;
  for(sq = 2; sq <= 10; sq++)
  {
    min = (sq-1)*(sq-1);
    min = min + (min+1)%2;
      for(i = min; i < sq*sq; i+=2)
      {
        for(j = 3; j <= sq; j+=2)
        {
          if (i%j == 0)
            bad;
        }
      }
  }
}/*===================================================*/
/*  ./Ch 7.OOD/1.Deck Of Cards/BlackJackCard.cpp  */
class BlackJackCard : public Card
{
public:
	BlackJackCard(int c, Suit *s) : Card(c, s)
	{
	}

	virtual int value()
	{
		if (isAce())
		{ 
			return 1;
		}
		else if (faceValue >= 11 && faceValue <= 13)
		{ 
			return 10;
		}
		else
		{ 
			return faceValue;
		}
	}

	virtual int minValue()
	{
		if (isAce())
		{ 
			return 1;
		}
		else
		{
			return value();
		}
	}

	virtual int maxValue()
	{
		if (isAce())
		{ 
			return 11;
		}
		else
		{
			return value();
		}
	}

	virtual bool isAce()
	{
		return faceValue == 1;
	}

	virtual bool isFaceCard()
	{
		return faceValue >= 11 && faceValue <= 13;
	}
};
/*===================================================*/
/*  ./Ch 7.OOD/1.Deck Of Cards/BlackJackHand.cpp  */
#include <vector>
#include <limits>

class BlackJackHand : public Hand<BlackJackCard*>
{
public:
	BlackJackHand()
	{

	}

	virtual int score()
	{
		std::vector<int> scores = possibleScores();
		int maxUnder = std::numeric_limits<int>::min();
		int minOver = std::numeric_limits<int>::max();
		for (auto score : scores)
		{
			if (score > 21 && score < minOver)
			{
				minOver = score;
			}
			else if (score <= 21 && score > maxUnder)
			{
				maxUnder = score;
			}
		}
		return maxUnder == std::numeric_limits<int>::min() ? minOver : maxUnder;
	}

private:
	void std::vector<int> possibleScores()
	{
		std::vector<int> scores;
		if (cards->size() == 0)
		{
			return scores;
		}
		for (BlackJackCard *card : cards)
		{
			addCardToScoreList(card, scores);
		}
		return scores;
	}

	void addCardToScoreList(BlackJackCard *card, std::vector<int> &scores)
	{
		if (scores.empty())
		{
			scores.push_back(0);
		}
		int length = scores.size();
		for (int i = 0; i < length; i++)
		{
			int score = scores[i];
			scores[i] = score + card->minValue();
			if (card->minValue() != card->maxValue())
			{
				scores.push_back(score + card->maxValue());
			}
		}
	}

public:
	virtual bool busted()
	{
		return score() > 21;
	}

	virtual bool is21()
	{
		return score() == 21;
	}

	virtual bool isBlackJack()
	{
		if (cards->size() != 2)
		{
			return false;
		}
		BlackJackCard *first = cards->get(0);
		BlackJackCard *second = cards->get(1);
		return (first->isAce() && second->isFaceCard()) || (second->isAce() && first->isFaceCard());
	}
};
/*===================================================*/
/*  ./Ch 7.OOD/1.Deck Of Cards/card.cpp  */
#include <string>
#include <vector>
#include <iostream>

class Card
{
private:
	bool available = true;

protected:
	int faceValue = 0;

	Suit *suit_Renamed;

public:
	Card(int c, Suit *s)
	{
		faceValue = c;
		suit_Renamed = s;
	}

	virtual int value() = 0;

	virtual Suit *suit()
	{
		return suit_Renamed;
	}

	
	virtual bool isAvailable()
	{
		return available;
	}

	virtual void markUnavailable()
	{
		available = false;
	}

	virtual void markAvailable()
	{
		available = true;
	}

	virtual void print()
	{
		std::vector<std::wstring> faceValues = {L"A", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"J", L"Q", L"K"};
		std::wcout << faceValues[faceValue - 1];
		switch (suit_Renamed)
		{
		case Club:
			std::wcout << std::wstring(L"c");
			break;
		case Heart:
			std::wcout << std::wstring(L"h");
			break;
		case Diamond:
			std::wcout << std::wstring(L"d");
			break;
		case Spade:
			std::wcout << std::wstring(L"s");
			break;
		}
		std::wcout << std::wstring(L" ");
	}
};
/*===================================================*/
/*  ./chapter-8-recursion-and-Dynamic-Programming/8-8-Permutation-with-dups-alt.cpp  */
#include <iostream>
#include <string>

using namespace std;

void printPerms(string, string = "");

int main()
{
  printPerms("abbc");
}

void printPerms(string remainder, string prefix)
{
  long length = remainder.length();
  
  if (!length) cout << prefix << endl;
  
  bool dup[128];
  
  memset(dup, false, sizeof(bool) * 128);
  
  for (int i = 0; i < length; ++i)
  {
    if (dup[remainder.at(i)]) continue;
    
    string str1 = i == 0 ? "" : remainder.substr(0,i);
    
    string str2 = i == length - 1 ? "" : remainder.substr(i+1,length);
    
    printPerms(str1 + str2, prefix + remainder.at(i));
  
    dup[remainder.at(i)] = true;
  }
}
/*===================================================*/
/*  ./chapter-8-recursion-and-Dynamic-Programming/8-1-Triple-Step.cpp  */
#include <vector>
#include <iostream>

using namespace std;

int countWays(int n , vector<int> v){

	if (n==0){
		return 1;
	}

	int sum = 0;

	for (int i = 0; i < v.size(); ++i)
	{
		if(n>=v[i]){
			sum = sum + countWays((n-v[i]),v);
		}	
	}
	return sum;
}

int main(int argc, char const *argv[])
{			

	vector<int> v;	// vector to store possible step sizes
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);

	int noOfWays = countWays(6,v);
	cout<<noOfWays<<endl;	// Total number of stairs = 6

	return 0;
}
/*===================================================*/
/*  ./chapter-8-recursion-and-Dynamic-Programming/8-2-Robot-in-a-Grid.cpp  */
//Question 8.2 C++ contrasting solution

#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

struct Point {
    int row;
    int column;
    Point(int r, int c) : row(r), column(c) {}
};



//////////////////////////////////////////////////////
//Implementation with memoization/caching
//////////////////////////////////////////////////////

//Checks if path is valid and simultaneously adds position to a result vector
bool getPath(int** matrix, int currRow, int currColumn, vector<Point*>& path, unordered_set<Point*>& duplicateVisits){
    
    //if out of bounds or curr position is off limits, return false
    if (currRow < 0 || currColumn < 0 || matrix[currRow][currColumn] == -1){
        return false;
    }
    
    Point* currPos = new Point(currRow, currColumn);
    
    //If we have already visited this position, then return false
    if (duplicateVisits.find(currPos) != duplicateVisits.end()){
        return false;
    }
    
    
    bool atOrigin = currRow == 0 && currColumn == 0;
    
    //Everytime robot moves up or left and it is a valid position, add the point to result vector
    if (atOrigin || getPath(matrix, currRow-1, currColumn, path, duplicateVisits) || getPath(matrix, currRow, currColumn-1, path, duplicateVisits)){
        path.push_back(currPos);
        return true;
    }
    
    //Keep track of already visited points
    duplicateVisits.insert(currPos);
    
    return false;
}

vector<Point*> getPath(int** matrix, int rows, int columns){
    //create result vector
    vector<Point*> path;
    
    //create unordered set to keep track of already visited points
    unordered_set<Point*> duplicateVisits;
    
    //Bounds checking
    if (rows != 0 || matrix != nullptr){
        //Start checking positions from bottom right to top left
        if (getPath(matrix, rows - 1, columns - 1, path, duplicateVisits)){
            return path;
        }
    }
    //Return an empty vector indicating path does not exist
    return path;
    
}


//////////////////////////////////////////////////////
//Implementation without memoization/caching
//////////////////////////////////////////////////////



//Checks if path is valid and simultaneously adds position to a result vector
//bool getPath(int** matrix, int currRow, int currColumn, vector<Point*>& path){
//    
//    //if out of bounds or curr position is off limits, return false
//    if (currRow < 0 || currColumn < 0 || matrix[currRow][currColumn] == -1){
//        return false;
//    }
//    
//    bool atOrigin = currRow == 0 && currColumn == 0;
//    
//    //Everytime robot moves up or left and it is a valid position, add the point to result vector
//    if (atOrigin || getPath(matrix, currRow-1, currColumn, path) || getPath(matrix, currRow, currColumn-1, path)){
//        Point* currPos = new Point(currRow, currColumn);
//        path.push_back(currPos);
//        return true;
//    }
//    return false;
//    
//    
//    
//    
//}
//
//vector<Point*> getPath(int** matrix, int rows, int columns){
//    //create result vector
//    vector<Point*> path;
//    
//    //Bounds checking
//    if (rows != 0 || matrix != nullptr){
//        //Start checking positions from bottom-right
//        if (getPath(matrix, rows - 1, columns - 1, path)){
//            return path;
//        }
//
//        
//    }
//    //Otherwise return an empty vector indicating path does not exist
//    return path;
// 
//}


int** createMatrix(int rows, int columns){
    //initialize number of rows
    int** matrix = new int*[rows];
    //in each row, add a new column array
    for (int i = 0; i < rows; ++i){
        matrix[i] = new int[columns];
    }
    //place values (1 indicates position is valid, -1 indicates it is invalid)
    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < columns; ++j){
            matrix[i][j] = 1;
        }
    }
    return matrix;
    
}

int main() {
    
    //create a 5x7 matrix (5 rows and 7 columns)
    int** matrix = createMatrix(5, 7);
    //set specific points as off-limits
    matrix[1][3] = -1;
    matrix[2][5] = -1;
    matrix[3][2] = -1;
    matrix[0][1] = -1;
    
    
    vector<Point*> path = getPath(matrix, 5, 7);
    if (path.size() == 0){
        cerr << "Path does not exist!" << endl;
        
    }
    else {
        for (int i = 0; i < path.size(); ++i){
            cout << "[" << path[i]->row << "]" << "[" << path[i]->column << "]" << endl;
        }
        
    }

}
/*===================================================*/
/*  ./chapter-8-recursion-and-Dynamic-Programming/8-7-Permutations-without-Dups.cpp  */
#include <iostream>
#include <vector>
#include <string>

using namespace std;

void findPermutations(string s, string curr,vector<string>& res){

	int n = s.size();
	if (!n)
	{
		res.push_back(curr);
		return;
	}

	for (int i = 0; i < n; ++i)
	{
		string first = curr+s.substr(i,1);
		string rem = s.substr(0,i) + s.substr(i+1,n-i-1);
		findPermutations(rem,first,res);
	}
}

int main()
{
	string s = "abc";
	string curr ="";
	vector<string> res;
	findPermutations(s,curr,res);
	for (int i = 0; i < res.size(); ++i)
		cout<<res[i]<<endl;
	return 0;
}
/*===================================================*/
/*  ./chapter-8-recursion-and-Dynamic-Programming/8-9-parens-alt.cpp  */
#include <iostream>
#include <vector>
#include <deque>

#define COUNT 3

using namespace std;

vector<deque<char>> * print_all_parentheses_internal(size_t count);

void print_all_parentheses(size_t count);

int main()
{
  print_all_parentheses(COUNT);
  return 0;
}

vector<deque<char>> * print_all_parentheses_internal(size_t count)
{
  if (count == 1)
  {
    deque<char> d;
    d.push_back('(');
    d.push_back(')');
    
    vector<deque<char>> * v = new vector<deque<char>>();
    
    v->push_back(d);
    
    return v;
  }
  
  vector<deque<char>> * v = print_all_parentheses_internal(count - 1);
  
  long size = v->size();
  
  for (int i = 0; i < size; ++i)
  {
    deque<char> tmp1 = v->at(i);
    deque<char> tmp2 = v->at(i);
    
    v->at(i).push_front(')');
    v->at(i).push_front('(');
    
    tmp1.push_front('(');
    tmp1.push_back(')');
    
    v->push_back(tmp1);
    
    if (i)
    {
      tmp2.push_back('(');
      tmp2.push_back(')');
      
      v->push_back(tmp2);
    }
  }
  
  return v;
}

void print_all_parentheses(size_t count)
{
  vector<deque<char>> * v = print_all_parentheses_internal(count);
  
  long size = v->size();
  
  for (int i = 0; i < size; ++i)
  {
    deque<char>::iterator it = v->at(i).begin();
    
    while (it != v->at(i).end()) cout << *it++;
    
    cout << endl;
  }
  
  delete v;
}
/*===================================================*/
/*  ./chapter-8-recursion-and-Dynamic-Programming/8-5-Recursive-Multiply.cpp  */
#include <vector>
#include <iostream>
using namespace std;

int multiply(int a, int b, vector<int>& dp)
{
	int bigger = a < b ? b:a;
	int smaller = a <b ? a:b;

	if (smaller==0)
		return 0;

	if (smaller==1)
		return bigger;

	if (dp[smaller]!=-1)
		return dp[smaller];

	int s = smaller >> 1; //divide by 2

	int side1 = multiply(s,bigger,dp);
	int side2 = 0;
	if (smaller%2)
		side2 = side1+bigger;
	else
	    side2 = side1;
	
	dp[smaller] = side2 +side1;
	return side1+side2;
}

int main()
{	
	int m = 7, n = 6;
	int smaller = m>n?n:m;
	int bigger = m>n?m:n;
	std::vector<int> dp(smaller+1,-1);
	cout<<multiply(smaller,bigger,dp)<<endl;
	return 0;
}
/*===================================================*/
/*  ./chapter-8-recursion-and-Dynamic-Programming/8-8-Permutation-with-dups.cpp  */
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

void findPermutations(string curr, map<char,int> m, vector<string>& res, int n){

	int curr_length = curr.length();
	if (n==curr_length){
		res.push_back(curr);
		return;
	}

	for (map<char,int>::iterator i = m.begin(); i!=m.end(); i++){
		if (i->second!=0){
			char c = i->first;
			string f = curr+c;
			i->second--;
			findPermutations(f,m,res,n);
			i->second++;
		}
	}
}

int main()
{
	string s = "abbc";
	string curr ="";
	map <char, int> m;
	int n = s.length();
	for (int i = 0; i < n; i++)
	{
		if (m.find(s[i])!=m.end())
			m.find(s[i])->second++;
		else
			m.insert(make_pair(s[i],1));
	}
	vector<string> res;
	findPermutations(curr,m,res,n);
	for (int i = 0; i < res.size(); i++)
	{
		cout<<res[i]<<endl;
	}
	return 0;
}
/*===================================================*/
/*  ./chapter-8-recursion-and-Dynamic-Programming/8-3-Magic-Index.cpp  */
#include <vector>
#include <iostream>
using namespace std;

int findMagicUtil(vector<int> v, int start, int end){
	
	int n = v.size();

	if (start<0  || end>=v.size()){		// index out of bounds
		return -1;
	}
	
	if (start>end){
		return -1;	
	}

	int mid = start + (end-start)/2;	// to avoid overflow. effectively, equals (start+end)/2
	if (mid==v[mid]){
	 	return mid;
	} 
	else{
		if (findMagicUtil(v,start,min(v[mid],mid))!=-1){
			return findMagicUtil(v,start,min(v[mid],mid));
		}
		
		return (findMagicUtil(v,max(v[mid],mid),end));
	}
}

int findMagic(vector<int> v){

	int n = v.size();
	if(n==0){
		return -1;
	}

	return findMagicUtil(v,0,n-1);
}

int main(int argc, char const *argv[])
{
	// Follow Up covered: Not Distinct
	vector<int> v; 
	v.push_back(-10);
	v.push_back(-1);
	v.push_back(2);
	v.push_back(2);
	v.push_back(2);
	v.push_back(3);
	v.push_back(5);
	v.push_back(8);
	v.push_back(9);
	v.push_back(12);
	v.push_back(13);

	cout<<findMagic(v)<<endl;
	return 0;
}
/*===================================================*/
/*  ./chapter-8-recursion-and-Dynamic-Programming/8-11-Coins.cpp  */
#include <iostream>
#include <vector>
using namespace std;

int ways(int amount, vector<int> denom){

	if (!amount){
		return 1;
	}
	
	int n = denom.size();
	if(n==1){
		if(amount%denom[0])
			return 0;
		else
			return 1;
	}
	int curr = denom[n-1]; 
	int sum = 0;
	denom.pop_back();
	sum += ways(amount,denom);
	while(amount>=curr){
		sum += ways(amount-curr,denom);
		amount = amount-curr;
	}
	return sum;
} 

int main(){	
	int amount = 8;
	vector<int> denom;
	denom.push_back(2);
	denom.push_back(3);

	cout<<ways(amount,denom)<<endl;
}
/*===================================================*/
/*  ./chapter-8-recursion-and-Dynamic-Programming/8-9-parens.cpp  */
#include <iostream>
#include <string>

void print_all_parentheses_internal(size_t count, size_t opened, size_t closed, std::string arangement){

  if(closed > opened){
    return;
  }
  else if(opened > count){
    return;
  }
  else if(arangement.size()==2*count){
    std::cout << arangement << std::endl;
  }

  print_all_parentheses_internal(count,opened+1,closed,  arangement+"(");
  print_all_parentheses_internal(count,opened  ,closed+1,arangement+")");
}

void print_all_parentheses(size_t count){
  print_all_parentheses_internal(count, 0, 0, "");
}

int main(int argc, const char* argv[]) {
  print_all_parentheses(3);
  return 0;
}
/*===================================================*/
/*  ./Ch 17. Hard/Q17_21_Volume_of_Histogram/histogramVolume.cpp  */
//g++ -std=gnu++11 -o histogramVolume histogramVolume.cpp
#include <vector>
#include <iostream>

typedef std::vector<unsigned> Histogram;

double getVolume(Histogram &histogram){
    // Skip 0's
    int i = 0;
    while ( i < histogram.size() - 1 && histogram[i] == 0 ) i++;
    int j = histogram.size() - 1;
    while ( j > i && histogram[j] == 0 ) j--;
    double volume = 0.0;
    while ( i != j ) {
        // Choose the smaller bar and advance to the center till we hit a bar
        // that is taller
        if ( histogram[i] < histogram [j] ){
            double height = histogram[i];
            double barsVolume = 0.0;
            int start = i++;
            while ( histogram[i] < height ) {
                barsVolume += histogram[i];
                i++;
            }
            volume += (i - start - 1) * height - barsVolume;
        } else {
            double height = histogram[j];
            double barsVolume = 0.0;
            int end = j--;
            while ( histogram[j] < height ){
                barsVolume += histogram[j];
                j--;
            }
            volume += (end - j - 1) * height - barsVolume;
        }
    }
    
    return volume;
}


int main(){
    Histogram histogram {0, 0, 4, 0, 0, 6, 0, 0, 3, 0, 8, 0, 2, 0, 5, 2, 0, 3, 0, 0};
    
    std::cout << "The volume of the histogram is: " << getVolume(histogram) << 
        std::endl;
}

/*===================================================*/
/*  ./Ch 17. Hard/Q17_01_Add_Without_Plus/addWithoutPlus.cpp  */
//g++ -std=gnu++11 -o addWithoutPlus addWithoutPlus.cpp
#include <iostream>

unsigned sum(unsigned x, unsigned y) {
    unsigned xor_result = x ^ y;
    unsigned carry = (x & y) << 1;
    if ( carry == 0 ) return xor_result;
    return sum(xor_result, carry);
}

int main(){
    std::cout << "The sum of 7 + 8 is: " << sum(7,8) << std::endl;
    std::cout << "The sum of 33 + 4 is: " << sum(33,4) << std::endl;
    std::cout << "The sum of 11 + 5 is: " << sum(5,11) << std::endl;
    std::cout << "The sum of 31 + 7 is: " << sum(31,7) << std::endl;
}
