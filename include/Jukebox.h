#ifndef JUKE_BOX_H
#define JUKE_BOX_H

#include <string> 
#include <queue>
#include <list>
#include <functional>
#include "RandomGenerator.h"

namespace ex{

	using namespace std;

	class Date{
	public:
		/* to be done*/
		bool operator<(const Date &rhs){ return true; }
		bool operator>(const Date &rhs){ return true; }
	};

	struct SongTags{
		SongTags(/*...*/){/*...*/ }
		~SongTags(){/*...*/}
		string songName;
		Date date;
		string composer;
		string artist;
		string additionalComments;
		string albumName;
		unsigned long duration;
	};
	struct SongData{
	public:
		SongData(/*...*/){/*...*/}
		~SongData(){/*...*/ }
		float samplingFrequency;
		unsigned long numSamples;
		unsigned char numChannels;
		string pathToData;

		void loadRawDataFromPath(){
			/*
			Use 'pathToData' to laod the song.
			*/
		}
		float *getRawData(){
			if (!rawData)
				throw string("Error at 'SongData::getRawData(...)': data for this son has not been loaded.\n");
			else
				return rawData;
		}

	private:
		/*This data is loaded only when the song is to be played*/
		float *rawData = nullptr;
	};

	class Album; //forward declared

	class Song{
	public:
		Song(){}
		~Song(){}
		
		Album *album;   //album to which this song belongs
		SongTags tags;  
		SongData *data; //data containing the signals to be played

	};

	//This class is actually not needed
	/*class Album{
	
	public:
		Album(){}
		~Album(){}		
	
	private:

		std::list<Song*> songs;

		string name;
		Date releaseDate;
		string artistName; //perofmers, i.e., Queen, Metallica, Britney Spears, etc.
		string recordingCompany;
		
	};*/

	/*This class will hold information about the songs, and allow to retrieve them
	in different sorting criteria*/
	class JukeboxBase{
	public:
		static JukeboxBase &getInstance(){
			static JukeboxBase instance;
			return instance;
		}
	
		std::list<Song*> *getSongsList(){
			return &songs;
		}

		std::list<Song*> *getOrderedBy(std::function<bool(Song *a, Song *b)> sortingFunc){
			songs.sort(sortingFunc);
			return &songs;
		}

		/*We could implement more sophisticated "gets", such as nested ordering, that is,
		first oredered by album, then by song name, and so on.*/
		std::list<Song*> *getByArtist(){
			return this->getOrderedBy([](Song *a, Song *b){return a->tags.artist < b->tags.artist; });
		}
		std::list<Song*> *getByDate(){
			return this->getOrderedBy([](Song *a, Song *b){return a->tags.date < b->tags.date; });
		}
		std::list<Song*> *getByName(){
			return this->getOrderedBy([](Song *a, Song *b){return a->tags.songName < b->tags.songName; });
		}
		std::list<Song*> *getByAlbum(){
			return this->getOrderedBy([](Song *a, Song *b){return a->tags.albumName < b->tags.albumName; });
		}
		std::list<Song*> *getByDuration(){
			return this->getOrderedBy([](Song *a, Song *b){return a->tags.duration < b->tags.duration; });
		}

		Song *getRandomSong(){
			size_t index = IntegerGenerator(0, songs.size()-1)();
			auto it = songs.begin();
			std::advance(it, index);
			return *it;
		}

	private:
		
		std::list<Song*> songs;

		JukeboxBase(){
			/*Load songs from some database*/
		}
		~JukeboxBase(){}

		void operator=(const JukeboxBase&);
		JukeboxBase(const JukeboxBase&);
	};

	/*This is a global class (singleton) because the jukebox has only one
	playlist.*/
	class Playlist{
	public:
		static Playlist &getInstance(){
			static Playlist instance;
			return instance;
		}

		/*After song is chosen and paid by the user, this method is called
		to add it to the queue*/
		void addToPlayList(Song* song){
			playlist.push(song);
		}

		/*The idea of this function is that it will be called by a thread
		everytime a song must be played, that is, when the previous has been,
		finished. If there are no songs in the queue to be played (no user requested
		anything), we return some random one to keep the 'beat going' :)! */
		Song *getNextToPlay(){
			if (!playlist.empty()){
				currentPlaying = playlist.front();
				playlist.pop();
			}
			else{
				currentPlaying = JukeboxBase::getInstance().getRandomSong();;
			}
			return currentPlaying;
		}
		Song *getCurrentPlaying(){ return currentPlaying; }

		size_t numSongsInQueue(){ return playlist.size(); }

	private:

		float princPerSong;

		Song* currentPlaying;
		std::queue<Song*> playlist;

		Playlist(){}
		~Playlist(){}

		void operator=(const Playlist&);
		Playlist(const Playlist&);
	};




};

#endif