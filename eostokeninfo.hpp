#include <eosiolib/types.hpp>
#include <eosiolib/currency.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/multi_index.hpp>
#include <cmath>

namespace eosio {

   /**
    *  This contract enables users to create an exchange between any pair of
    *  standard currency types.
    *
    *  Users must deposit funds into the exchange before they can trade on the
    *  exchange.
    */
   class eostokeninfo {
      private:
         account_name      _this_contract;

         //update the fanclub
         void update_fanclub( account_name owner, extended_asset quantity, std::string memo );
         //create the token
         void create_token( account_name owner, extended_asset quantity );

         //create the key to look up
         static uint64_t get_asset_key(extended_asset quantity) {
           return quantity.get_extended_symbol().value + quantity.contract;
         }

         //@abi table
         struct token {
            account_name      issuer = 0;
            extended_asset    max_supply;
            std::string       description = "";
            std::string       purpose = "";
            std::string       website = "";
            std::string       airgrab_json = "";
            std::string       ico_json = "";
            account_name      fanclub_winner = 0;
            extended_asset    fanclub_bid;
            std::string       fanclub_comment = "";

            auto primary_key()const { return get_asset_key(max_supply); }
         };

         typedef eosio::multi_index<N(tokens), token> tokens;

         //@abi table
         //we need to mimic the stats table to find the issuer
         struct currency_stats {
            asset          supply;
            asset          max_supply;
            account_name   issuer;

            uint64_t primary_key()const { return supply.symbol.name(); }
         };

         typedef eosio::multi_index<N(stat), currency_stats> stats;

      public:
         eostokeninfo( account_name self )
         :_this_contract(self)
         {}

         void set( account_name issuer, extended_asset max_supply, std::string description, std::string purpose, std::string website, std::string airgrab_json, std::string ico_json);
         void remove( account_name issuer, extended_asset max_supply );
         void on( const currency::transfer& t, account_name code );

         void apply( account_name contract, account_name act );
   };
} // namespace eosio
