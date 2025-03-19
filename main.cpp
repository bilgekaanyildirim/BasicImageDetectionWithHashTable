#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#define ITEM_NOT_FOUND "Item not found"

// HashTable class to manage a hash table with rehashing and custom encoding
class HashTable
{
public:
    // Constructor to initialize hash table with a given size (default is 101)
    explicit HashTable(int tableSize = 101) : size(0), array(nextPrime(tableSize)) {}

    // Removes an element from the hash table
    void remove(const string & element)
    {
        int pos = findPosition(element);

        if (isActive(pos))
            array[pos].info = DELETED;
    }

    // Finds an element in the hash table and returns its encoded image or ITEM_NOT_FOUND
    const string find(const string & element) const
    {
        int pos = findPosition(element);

        if (isActive(pos))
        {
            return array[pos].encodedImage;
        }

        return ITEM_NOT_FOUND;
    }

    // Inserts an encoded image into the hash table
    void insert(const string & encodedImage)
    {
        int pos = findPosition(encodedImage);

        if (isActive(pos))
            return;

        array[pos] = HashEntry(encodedImage, ACTIVE);

        // Rehash if the table is more than half full
        if (++size >= array.size() / 2)
            rehash();
    }

    // Prints the hash table entries
    void print() const
    {
        for (int i = 0; i < array.size(); ++i)
        {
            cout << i << ": " << array[i].encodedImage << array[i].info << endl;
        }
    }

    // Entry types in the hash table
    enum EntryType { ACTIVE, EMPTY, DELETED };

private:
    // Hash entry struct to store encoded image and its status
    struct HashEntry
    {
        string encodedImage;
        EntryType info;

        HashEntry(const string & encodedImage, EntryType info = EMPTY) : encodedImage(encodedImage), info(info){}
        HashEntry() : encodedImage(""), info(EMPTY){}
    };

    vector<HashEntry> array; // The hash table array
    int size;               // Current size of the hash table

    // Hash function to calculate hash value
    int hashFunc(const string & encodedString, const int & arraySize) const
    {
        return (encodedString.length() % arraySize);
    }

    // Checks if a position is active
    bool isActive(int currentPos) const
    {
        return array[currentPos].info == ACTIVE;
    }

    // Finds the position of an element in the hash table
    int findPosition(const string & element) const
    {
        int arraySize = array.size();
        int p = hashFunc(element, arraySize);

        while (array[p].info != EMPTY && array[p].encodedImage != element)
        {
            p++;

            if (p >= arraySize)
            {
                p -= arraySize;
            }
        }

        return p;
    }

    // Rehashes the hash table when it gets too full
    void rehash()
    {
        vector<HashEntry> oldArray = array;

        array.resize(nextPrime(2 * oldArray.size()));

        for (int j = 0; j < array.size(); ++j)
        {
            array[j].info = EMPTY;
        }

        size = 0;

        for (int i = 0; i < oldArray.size(); ++i)
        {
            if (oldArray[i].info == ACTIVE)
                insert(oldArray[i].encodedImage);
        }
    }

    // Finds the next prime number greater than or equal to n
    int nextPrime(int n)
    {
        if (n % 2 == 0)
            n++;

        while(!isPrime(n))
            n += 2;

        return n;
    }

    // Checks if a number is prime
    bool isPrime(int n)
    {
        if( n == 2 || n == 3 )
            return true;

        if( n == 1 || n % 2 == 0 )
            return false;

        for( int i = 3; i * i <= n; i += 2 )
            if( n % i == 0 )
                return false;

        return true;
    }
};

#define IMAGE_FILENAME(number) "image" + number + ".txt"
#define QUERY_FILENAME(number) "query" + number + ".txt"
#define IMAGE_SIZE 28

int main()
{
    string input;
    HashTable hashTable(30); // Initialize hash table with size 30
    char ch;

    // Function prototypes
    string RunLengthEncoding(const string & flattened);
    string RunLengthDecoding(const string & encodedImage);

    // Loop to insert images into the hash table
    while(input != "query")
    {
        cout << "Enter image number to insert into the hash table (or 'query' to continue): " << endl;
        cin >> input;

        if (input == "query")
            break;

        ifstream file;
        file.open(IMAGE_FILENAME(input)); // Open the corresponding image file

        string flattenedImage;
        string line;

        // Read the binary image from the file
        while (file.get(ch))
        {
            if (ch == '0' || ch == '1')
                flattenedImage += ch;
        }

        string encodedImage = RunLengthEncoding(flattenedImage); // Encode the binary image

        hashTable.insert(encodedImage); // Insert the encoded image into the hash table

        cout << "Image " << input << " inserted into the hash table." << endl;
    }

    // Loop to query images in the hash table
    while(input != "exit")
    {
        cout << "Enter image number to query (or 'exit' to quit): " << endl;
        cin >> input;

        if (input == "exit")
            break;

        ifstream file;
        file.open(QUERY_FILENAME(input)); // Open the corresponding query file

        string flattenedImage;
        string line;

        // Read the binary image from the file
        while (file.get(ch))
        {
            if (ch == '0' || ch == '1')
                flattenedImage += ch;
        }

        string encodedImage = RunLengthEncoding(flattenedImage); // Encode the binary image

        string turnedImage = hashTable.find(encodedImage); // Search for the encoded image in the hash table

        if (turnedImage == ITEM_NOT_FOUND)
        {
            cout << "No match for the image with encoding: " << encodedImage << endl;
            continue;
        }

        cout << "RLE String for " << QUERY_FILENAME(input) << " found in hash table." << endl;

        string decodedImageFlat = RunLengthDecoding(encodedImage); // Decode the found image

        // Display the decoded image in a 28x28 grid
        for (int i = 0; i < IMAGE_SIZE; ++i)
        {
            for (int j = 0; j < IMAGE_SIZE; ++j)
            {
                cout << decodedImageFlat[j + i * IMAGE_SIZE];
            }
            cout << endl;
        }
    }

    cout << "Exiting the program!";
}

// Function to perform Run-Length Encoding on a flattened binary image
string RunLengthEncoding(const string & flattened)
{
    string rle;
    int count = 1;
    char current = flattened[0];

    for (int i = 1; i < flattened.size(); i++)
    {
        if (flattened[i] == current)
        {
            count++;
        }
        else
        {
            rle += to_string(count) + (current == '1' ? "W" : "B");
            current = flattened[i];
            count = 1;
        }
    }
    rle += to_string(count) + (current == '1' ? "W" : "B");

    return rle;
}

// Function to perform Run-Length Decoding on an encoded image
string RunLengthDecoding(const string & encodedImage)
{
    string decodedImageFlat = "";
    int i = 0;

    while(i < encodedImage.length())
    {
        string countStr = "";

        while(isdigit(encodedImage[i]))
        {
            countStr += encodedImage[i];
            i++;
        }

        int count = stoi(countStr);

        char WB = encodedImage[i];
        char binary = WB == 'W' ? '1' : '0';

        for (int j = 0; j < count; ++j)
        {
            decodedImageFlat += binary;
        }
        i++;
    }
    return decodedImageFlat;
}
