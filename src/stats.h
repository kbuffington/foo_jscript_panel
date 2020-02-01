#pragma once

namespace stats
{
	struct fields
	{
		size_t playcount = 0;
		size_t loved = 0;
		pfc::string8_fast first_played;
		pfc::string8_fast last_played;
		size_t rating = 0;
	};

	using hash_set = std::set<metadb_index_hash>;

	bool hashHandle(const metadb_handle_ptr& handle, metadb_index_hash& hash);
	fields get(metadb_index_hash hash);
	metadb_index_manager::ptr theAPI();
	void get_hashes(metadb_handle_list_cref handles, hash_set& hashes);
	void set(metadb_index_hash hash, fields f);
}
