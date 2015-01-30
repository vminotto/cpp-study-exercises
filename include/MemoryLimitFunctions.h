#ifndef MEMORY_LIMIT_FUNCTIONS_H
#define MEMORY_LIMIT_FUNCTIONS_H

#include <cstdint>
#include <fstream>
#include <string>
#include <algorithm>
#include <bitset>
#include <vector>
#include <unordered_set>
#include "DirectedGraph.h"

namespace ex{

	/*10.7 Imagine a web server for a simplified search engine. This system has WO machines to
	respond to search queries,which may then call out using processSearch (string
	query) to another cluster of machines to actually get the result. The machine
	which responds to a given query is chosen at random, so you can not guarantee that
	the same machine will always respond to the same request. The method process -
	Search is very expensive. Design a caching mechanism to cache the results of the
	most recent queries. Be sure to explain how you would update the cache when data
	changes.
	
	One way would be to keep a hash_table containg the results of previously made queries. One
	identical copy of the hash_table could be kept on each of the 100 computers, such that when a 
	new query is made, the key (query string) and result are broadcasted to all other 99 computers
	to update their version of the hash_table. If a requested has already been made, simply
	return the value for that in the hash_table. As a second option, if all queries pass through the server
	before going to one of the 100 computers, we could keep a single hash_table in the server itself, thus,
	avoiding the need for broadcasting. A third option is to divide up the cache, such that each machine holds a different part
	of it. Then, when machine i needs to look up the results for a query, machine i would
	figure out which machine holds this value, and then ask this other machine (machine j)
	to look up the query in j's cache.
	But how would machine i know which machine holds this part ofthe hash table?
	One option is to assign queries based on the formula hash (query) % N.Then, machine
	i only needs to apply this formula to know that machine j should store the results for
	this query.

	In order to detect for data changes some options may be considered:
	If the returned result contains metadata about the last modification of the data, we may simply compare the results.
	We could do this for other metadata as well, such as the size of the returned result, type of data returned etc.
	In case no metadata exists, another option would be to compare bit-by-bit of the data in the cache and of the returned
	data, but this would be slow. We coudl, therefore, hash both values, and compare them.

	One way to detect if a given data has change, is to keep a counter of how many times the cached result has been returned.
	After it reaches a given limit, we re-query that given query and compare the results (as described above), if the returned
	one is more recent, we update the hash-table. Besides using a counter, we have a timestamp on each of the cached results,
	such that we automatically re-query them after a given period of time. Furthermore, we could also elimited cached results that
	have not been queried frequently and that are above a given timestamp, so that we free some space in the hash_table.
	*/

	/*10.6 You have 10 billion URLs. How do you detect the duplicate documents? In this case,
	assume that "duplicate" means that the URLs are identical.
	
	10 billion URLS ~ 4 terabytes of data
	1 url = 100 chars = 400 bytes
	10 000 000 000 urls = 1e10 * 400 = 4e12 = 4 000 000 000 000 ~ 4 TB

	If we had enough memory space, we could load all files into a ram-allocated hash-table, thus automatically
	handling duplicates. Since this is not a possibility, we could opt to use the disk, assuming we have a 4TB+
	hard drive. If we do not have such disk, we could use multiple machiens.
	
	Solution 1 - Hard Drive:
	We could iterate through the entire file, splitting it into smaller files of ~1GB, thus having a total of 4000
	files. The idea is to make duplicate URLs land on the same file. Also, we have to evenly distribute URLs across
	all smaller files. One good option would be to map different URLs into same filenames , and use the files with 
	those names to store the url.
	One way to do this would be to name each file [x].txt where [x] = [hash(URL) % 4000]. The modulo operator would
	make sure we evenly distribute the URLs across all files, and the HASH function would make sure all duplicates 
	end up landing in the same file. The hashing function could be any simple existing string hasher  as in 
	std::hash<std::string>(). After all URLs are distributed, we iterate through all sub-files, load them into
	memory, add all its URLs to a HashTable and re-record them. This way, duplicates are removed easely.
	
	Solution 2 - Multiple machiens
	Repeat the above process, but splitting the smaller files into different machines, instead of on the same hard
	drive.

	Advantages: The second pass could run in parallel, resulting in a faster solution for very large ammount of data.
	
	Disadvantages: Slower for smaller data, and introducing multiple machines makes the system more prone to errors.
	There is always a chance that one machine might fail, or we cannot estabilish connection to them. We might need
	to create some sort of error-handling module. Also, simply involving multiple machines increases the complexity
	of the problem.

	*/


	/*10.5 If you were designing a web crawler, how would you avoid getting into infinite loops?
		
	This problem may be viewed as a directed graph traversal where we do not want to visit the same node
	twice. In a graph data structure, we may do this by marking each node as visited. In a web crawler, since
	we may not create such structures (and addresses are divided among different computers) we may just keep a record
	of the already visited web-pages. This record may be the same one used for later indexing the pages.
	
	Problem 1: 
	Using the URL to differ between webpages is not always the best option. URLs with wrong parameters are
	generally reduced to the base one, as in, 'www.google.com/foo=blabla' is reduced to 'www.google.com'. 
	This could case an infinit loop if a webpage has broken link that would actually be interpreted 
	differently (corrected)	by the browser.
	Furthermore, two webpages with different URLs may lead to the same HTML retrieval, causing the same
	problem as above.

	Problem 2:
	Using the content of the HTML to differ between webpages might seem a good idea, but would drastically
	fail if a webpage has some random-generated element. That is, consecutive visits to the same URL would
	generate different HTMLs, which could cause an infinit loop.

	Solution:
	The best way is to index web-pages by a custom score that is created from the page's URL and content. 
	Pages with identical scores would therefore be avoided, and pages with simillar scores would have
	reduced priority for crawling.
	*/

	template<class T> void performWebCrawl(ex::GraphNode<T> *srcPage, std::unordered_set<T> &pages, int maxIndexingPermitted){
		
		/*If maxIndexingPermitted == 0, there is no limit for indexing.
		If maxIndexingPermitted > 0 and this limit is reached, we stop the web-crawling process*/
		if (maxIndexingPermitted > 0 && pages.size() >= maxIndexingPermitted)
			return;

		/*If this page has already been visited, we do not proceed (to avoid infinit loops)*/
		if (!pages.insert(srcPage->value).second)
			return;

		auto &nextPages = srcPage->getConnections();

		for (auto &nPage : nextPages)
			performWebCrawl(nPage, pages, maxIndexingPermitted);
	}
	template<class T> std::unordered_set<T> performWebCrawl(ex::GraphNode<T> *srcPage, int maxIndexingPermitted){
		std::unordered_set<T> pages;
		performWebCrawl(srcPage, pages, maxIndexingPermitted);
		return pages;
	}

/*
10.3 Given an input file with four billion non-negative integers, provide an algorithm
to generate an integer which is not contained in the file. Assume you have 1 GB of
memory available for this task.
FOLLOW UP
What if you have only 10 MB of memory? Assume that all the values are distinct and
we now have no more than one billion non-negative integers.
               (kB)   (mB)   (gB)
4000000000*4 / 1024 / 1024 / 1024 = 14.9 GigaBytes

1 GB = 8 589 934 592 bits
2^32 = 4 294 967 296 possible uints

Re-scalling this problem to make it possible to fit in a file:

4000 integers
1kB  maximum memory = 8192 bits


Follow UP
10mb memory limit for 1 billion integers.

10mb = maximum of   2 621 440 integers = 83 886 080 bits
1 billion   =   1 000 000 000 integers
max pos int =   2 147 483 647 
*/

	uint32_t getAbsentPositiveInt(std::string path){
		std::ifstream input(path);

		/*This command ins not obligatory*/
		std::skipws(input);

		std::bitset<UINT32_MAX> *mask = new std::bitset<UINT32_MAX>();

		std::for_each(std::istream_iterator<uint32_t>(input), istream_iterator<uint32_t>(), [&](uint32_t val)
		{
			mask->set(val, true);
		});

		for (uint32_t i = 0; i < UINT32_MAX; i++){
			if (!mask->at(i)){
				delete mask;
				return i;
			}
		}

		delete mask;
		return 0;
	}
	
	template <uint32_t memoryLimitInBits> uint32_t getAbsentPositiveInt(std::string path)
	{
		std::ifstream input(path);
		/*This command ins not obligatory*/
		std::skipws(input);

		/*If more memory available than the amount of values, we call the other version of this method*/
		const uint32_t maxValue = (1 << (sizeof(uint32_t)* 8 - 1)) - 1;

		if (memoryLimitInBits > maxValue)
			return getAbsentPositiveInt(path);


		/*Maximum ints allocable with the amount of memory available*/
		const uint32_t memoryLimitInInts = memoryLimitInBits / (sizeof(uint32_t)*8);
		/*Here we divide by two to compensate for the temporary variables used*/
		const uint32_t numBuckets		 = memoryLimitInInts >> 1;
		/*Maximum value each bucket can count to*/
		const uint32_t maxCountPerBucket = maxValue / numBuckets + 1;

		/*Count of each bucket*/
		uint32_t *count = new uint32_t[numBuckets]();
	
		/*Pass 1: count the ocurrence of values in each bucket*/
		std::for_each(std::istream_iterator<uint32_t>(input), istream_iterator<uint32_t>(), [&](uint32_t val)
		{
			uint32_t bucketNum = val / maxCountPerBucket;
			count[bucketNum]++;
		});

		/*Chose the bucket with less counts, which is the one with missing integer ocurrences. Ohter buckets
		with count < maxCountPerBucket are also possible, but chosing the one with less ocurrences implies in 
		less computational cost for the next steps.*/
		uint32_t chosenBucket = std::min_element(count, count + numBuckets) - count;

		/*Minimum and maximum values that may ocur in the chosen bucket.*/
		uint32_t min = maxCountPerBucket*chosenBucket;
		uint32_t max = (maxCountPerBucket + 1)*chosenBucket - 1;
		
		delete[] count;

		/*Rewinding the read pointer of the input file, so we can iterate through it again. Both functions below
		must be called (in this order).*/
		input.clear();
		input.seekg(input.beg);

		std::bitset<maxCountPerBucket> *mask = new std::bitset<maxCountPerBucket>();
		/*Pass 2: mark as ocurred each value in the bucket using a bit array*/
		std::for_each(std::istream_iterator<uint32_t>(input), istream_iterator<uint32_t>(), [&](uint32_t val)
		{
			if (val >= min && val <= max){
				mask->set(val - min, true);
			}
		});

		for (uint32_t i = 0; i < mask->size(); i++){
			if (!mask->at(i)){
				delete mask;
				return i+min;
			}
		}

		return 0;
	}

	/*10.4 You have an array with all the numbers from 1 to N, where N is at most 32, 000. The
	array may have duplicate entries and you do not know what N is. With only 4 kilobytes
	of memory available, how would you print all duplicate elements in the array ?
	
	4kb of data = 1024 * 4 * 8 = 32768 bits
	4kb of data = 1024 / 4     = 256 ints
	*/
	/*This version prints each duplicates only once (even if it appears more than twice), without extra
	memory usage but at high computational cost.*/
	void printDuplicates(std::vector<int> vec){

		std::sort(vec.begin(), vec.end());

		auto end = vec.end();
		auto pos = adjacent_find(vec.begin(), vec.end());

		while (pos != end){
			int val = *pos;
			cout << val << endl;
			while (*pos == val){
				++pos;
			}
			pos = adjacent_find(pos, vec.end());
		}
	}

	/*This version prints duplicates for as many times as they appear, and uses extra memory.
	It runs at O(N) time.*/
	template<uint32_t maxVal> void printDuplicates(std::vector<int> vec){

		/*Initialized to all 0*/
		std::bitset<maxVal> mask;

		for (int val : vec){
			if (!mask.at(val)) //mark first ocurrence
				mask.set(val, true);
			else               //if already ocurred, print it
				cout << val+1 << endl; //print +1 because vec starts at 1
		}
			

	}

};

#endif