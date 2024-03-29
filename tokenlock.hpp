#include <eosio/eosio.hpp>
#include <eosio/symbol.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>

using namespace eosio;
using namespace std;

class [[eosio::contract]] tokenlock : public contract {
  public:
    tokenlock(name receiver, name code, datastream<const char*> ds )
        : contract(receiver, code, ds),
        _lockups(receiver, code.value) {}
        
    const name TOKEN_CONTRACT = "osbio.token"_n;
    
    [[eosio::action]] void transferlock(name from,
                      name to,
                      asset quantity,
                      string memo,
                      uint64_t unlock_timestamp);
    [[eosio::action]] void claim(name receiver, uint64_t no);
    [[eosio::action]] void claimall(name receiver);
    [[eosio::action]] void currenttime();
    
  private:
    struct [[eosio::table]] lockup {
        uint64_t no;
        name sender;
        name receiver;
        asset token;
        string memo;
        uint64_t lock_begin = 0;
        uint64_t lock_end = 0;
        uint64_t claim = 0;
        string tx_id_lockup;
        string tx_id_claim;
        uint64_t primary_key() const { return no; }
        uint64_t secondary_key() const { return receiver.value; }
    };
    
    checksum256 get_trx_id();
    
    typedef multi_index<"lockup"_n, lockup,
    indexed_by<"getreceiver"_n, const_mem_fun<lockup, uint64_t, &lockup::secondary_key>>> lockup_table;

    lockup_table _lockups;
};