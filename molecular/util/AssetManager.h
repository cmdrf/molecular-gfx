/*	AssetManager.h
	Copyright 2012-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <molecular/util/Hash.h>
#include <molecular/util/NonCopyable.h>
#include <unordered_map>
#include <cassert>
#include <array>


namespace molecular
{
namespace gfx
{

inline Hash MakeHash(const std::string& string)
{
	return HashUtils::MakeHash(string);
}

constexpr Hash MakeHash(Hash hash)
{
	return hash;
}

/// Loads assets when they are needed, and unloads them when they are not needed anymore
template<class T, int lodLevels, bool mipmapStyle, class Location = std::string>
class AssetManager : NonCopyable
{
public:
	class Asset;

	/// Interface to an asset loader
	/** This has to be implemented for each new AssetManager class. */
	class Loader
	{
	public:
		virtual T Create() = 0;
		virtual void Destroy(T& asset) = 0;
		virtual void StartLoad(Asset& asset, unsigned int minLevel, unsigned int maxLevel) = 0;
		virtual void Unload(T& asset, unsigned int minLevel, unsigned int maxLevel) = 0;

	protected:
//		typedef Asset Asset;
		static const unsigned int kLodLevels = lodLevels;

	};

	AssetManager(Loader& loader) : mLoader(loader) {}
	virtual ~AssetManager();

	class Asset
	{
		friend class AssetManager;
	public:
		/// Initiate loading if necessary and return underlying object
		/** Using specific texture LOD levels is not supported on GLES. */
		T Use(unsigned int lodLevel = 0);

		enum State
		{
			kLoading,
			kLoaded,
			kNotLoaded,
			kFailed
		};

		/** Used by Loader classes. */
		void SetState(unsigned int lodLevel, State state) {assert(lodLevel < lodLevels); mStates[lodLevel] = state;}
//		State GetState(unsigned int lodLevel) {return mStates[lodLevel];}

		/// Get the underlying object
		/** Does not initiate loading. Used by Loader classes. */
		T& GetAsset() {return mAsset;}

		/** Used by Loader classes. */
		const Location& GetLocation() {return mLocation;}

	private:
		Asset(Loader& loader, const Location& location) :
			mLocation(location),
			mAsset(loader.Create()),
			mLoader(loader)
		{
			mLastUsed.fill(0);
			mStates.fill(kNotLoaded);
		}
		Asset(Asset&) = delete;

		~Asset()
		{
			mLoader.Destroy(mAsset);
		}
		Asset& operator=(Asset&) = delete;

		std::array<int, lodLevels> mLastUsed; // Frame counter
		std::array<State, lodLevels> mStates;
		Location mLocation;
		T mAsset;
		Loader& mLoader;
	};

	void Update(int framecounter);

	/// Get Asset object for given location
	/** Loading is not initiated. Use Asset::Use for that. */
	Asset* GetAsset(const Location& location);

protected:


private:
	Loader& mLoader;

	/** Entries are currently never removed during normal operation. */
	std::unordered_map<Hash, Asset*> mAssets;
};

template<class T, int lodLevels, bool mipmapStyle, class Location>
typename AssetManager<T, lodLevels, mipmapStyle, Location>::Asset* AssetManager<T, lodLevels, mipmapStyle, Location>::GetAsset(const Location &location)
{
	Hash hash = MakeHash(location);
	auto it = mAssets.find(hash);
	if(it == mAssets.end())
	{
		Asset* asset = new Asset(mLoader, location);
		mAssets.insert(std::make_pair(hash, asset));
		return asset;
	}
	else
	{
		assert(it->second);
		return it->second;
	}
}

/*
template<class T, int lodLevels, bool mipmapStyle>
void AssetManager<T, lodLevels, mipmapStyle>::ReturnAsset(Asset* asset)
{
	assert(asset);
	asset->mUsageCounter--;
}
*/

template<class T, int lodLevels, bool mipmapStyle, class Location>
AssetManager<T, lodLevels, mipmapStyle, Location>::~AssetManager()
{
	for(auto& asset: mAssets)
		delete asset.second;
}

template<class T, int lodLevels, bool mipmapStyle, class Location>
T AssetManager<T, lodLevels, mipmapStyle, Location>::Asset::Use(unsigned int lodLevel)
{
	assert(lodLevel < lodLevels);

	unsigned int maxLevel = lodLevel;
	if(mipmapStyle)
	{
		while(maxLevel < lodLevels && mStates[maxLevel] == kNotLoaded)
			maxLevel++;
		maxLevel--;
	}

	if(mStates[lodLevel] == kNotLoaded)
	{
		mStates[lodLevel] = kLoading;
		mLoader.StartLoad(*this, lodLevel, maxLevel);
	}

	return mAsset;
}

template<class T, int lodLevels, bool mipmapStyle, class Location>
void AssetManager<T, lodLevels, mipmapStyle, Location>::Update(int /*framecounter*/)
{
	// TODO
}

}
}

#endif // ASSETMANAGER_H
