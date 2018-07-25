#include <math.h>
#include "eostokeninfo.hpp"

namespace eosio {

   //@abi action
   void eostokeninfo::on( const currency::transfer& t, account_name code ) {
      if( t.to == _this_contract ) {
         auto a = extended_asset(t.quantity, code);
         eosio_assert( a.is_valid(), "invalid quantity in transfer" ); //maybe redundant
         eosio_assert( a.amount != 0, "zero quantity is disallowed in transfer");
         create_token( t.from, a );
         update_fanclub( t.from, a, t.memo);
      }
   }

   //@abi action
   void eostokeninfo::set( account_name issuer, extended_asset max_supply, std::string description, std::string purpose, std::string website, std::string airgrab_json, std::string ico_json) {
     create_token( issuer, max_supply );

     print("Setting details for ", max_supply);

     //lets find if we have an entry
     tokens tokens_tbl(_this_contract, _this_contract);
     auto key = get_asset_key(max_supply);
     auto token = tokens_tbl.find( key );

     //should always exist because we create it first
     eosio_assert(token != tokens_tbl.end(),"token doesn't exist");

     require_auth(token->issuer);

      tokens_tbl.modify( token, issuer, [&]( auto& a ) {
        a.description = description;
        a.purpose = purpose;
        a.website = website;
        a.airgrab_json = airgrab_json;
        a.ico_json = ico_json;
      });
   }

   //@abi action
   void eostokeninfo::remove( account_name issuer, extended_asset max_supply) {
     print("Removing ", max_supply);

     //lets find if we have an entry
     tokens tokens_tbl(_this_contract, _this_contract);
     auto key = get_asset_key(max_supply);
     auto token = tokens_tbl.find( key );
     //should always exist because we create it first
     eosio_assert(token != tokens_tbl.end(),"token doesn't exist");
     require_auth(token->issuer);
     tokens_tbl.erase(token);
   }

   //PRIVATE FUNCTION
   void eostokeninfo::create_token( account_name owner, extended_asset quantity )
   {
      require_auth(owner);
      eosio_assert( quantity.is_valid(), "invalid quantity" );

      //lets find if we have an entry
      tokens tokens_tbl(_this_contract, _this_contract);
      auto key = get_asset_key(quantity);
      auto token = tokens_tbl.find( key );
      if( token == tokens_tbl.end() ) {
        //token doesn't exist yet - let's add it
        //find the currency
        print("Congrats! You are the first to add ", quantity);
        stats stats_tbl( quantity.contract, quantity.symbol.name() );
        auto stat = stats_tbl.find( quantity.symbol.name() );
        eosio_assert(stat != stats_tbl.end(),"stat table doesn't exist for this token");
        //token is legit
        tokens_tbl.emplace( owner, [&]( auto& a ){
          a.issuer = stat->issuer;
          a.max_supply = extended_asset(stat->max_supply,quantity.contract);
        });
      }
   }

   void eostokeninfo::update_fanclub( account_name owner, extended_asset quantity, std::string memo )
   {
      require_auth(owner);

      print("Fanclub contestant ");
      printn(owner);
      print(" bid of ", quantity, " with memo ", memo);

      //lets find if we have an entry
      tokens tokens_tbl(_this_contract, _this_contract);
      auto key = get_asset_key(quantity);
      auto token = tokens_tbl.find( key );

      //should always exist because we create it first
      eosio_assert(token != tokens_tbl.end(),"token doesn't exist");
      eosio_assert( quantity.amount - token->fanclub_bid.amount > (token->fanclub_bid.amount / 10), "must increase bid by 10%" );

       tokens_tbl.modify( token, owner, [&]( auto& a ) {
         a.fanclub_winner = owner;
         a.fanclub_bid = quantity;
         a.fanclub_comment = memo;
       });
   }

   #define N(X) ::eosio::string_to_name(#X)


   void eostokeninfo::apply( account_name contract, account_name act ) {

      if( act == N(transfer) ) {
         on( unpack_action_data<currency::transfer>(), contract );
         return;
      }

      if( contract != _this_contract )
         return;

      auto& thiscontract = *this;
      switch( act ) {
         EOSIO_API( eostokeninfo, (set)(remove) )
      };
   }

} /// namespace eosio



extern "C" {
   [[noreturn]] void apply( uint64_t receiver, uint64_t code, uint64_t action ) {
      eosio::eostokeninfo  ex( receiver );
      ex.apply( code, action );
      eosio_exit(0);
   }
}
