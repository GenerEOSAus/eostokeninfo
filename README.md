# eostokeninfo
Keep track of EOS tokens on chain

Are you tired of not knowing what tokens and airdrops are available? Scouring the internet for details?

No longer!
***With eostokeninfo eos tokens are simple and on chain***

## Get the list of tokens
`cleos get table eostokeninfo eostokeninfo tokens`

This returns any registered tokens with the following table

|column|description|
|------|-----------|
|issuer|The issuing account of the token|
|max_supply|The max supply of the token, in `extended_asset` form|
|description|A description of the token *provided by the issuer*|
|purpose|The purpose of the token *provided by the issuer*|
|website|A website url token details *provided by the issuer*|
|airgrab_json|A json structure that tells people how they can airgrab the token *provided by the issuer*|
|ico_json|A json structure that tells people how they can participate in the ico *provided by the issuer*|
|fanclub_winner|This account is the tokens biggest fan|
|fanclub_bid|The current highest fanclub bid|
|fanbclub_comment|A comment the biggest fan wants to share with the world|

## How to add or update token listings
1. As the issuer of the token

   `cleos push action eostokeninfo set '{"issuer":"eosio.token","max_supply":{"quantity":"10000000000.0000 EOS","contract":"eosio.token"},"description":"","purpose":"","website":"","airgrab_json":"","ico_json":""}' -p eosio.token`
   
   * This command can be run anytime you want to update the token
   
1. As a fan of the token

   * Just transfer some quantity of the token to eostokeninfo with a memo you want the world to see.
      Congratulations on being the first fanclub member!
   * If you aren't the first, you will have to increase the quantity by 10% of the current fanclub_bid to take over.
   * ALL proceeds from the fanclub will support the GenerEOS community giving program.
   * All winning fanclub bids are non-refundable - thanks for helping charity and the community.

## How to remove the token listing
1. Only the issuer can do this

   `cleos push action eostokeninfo remove '{"issuer":"eosio.token","max_supply":{"quantity":"10000000000.0000 EOS","contract":"eosio.token"}}' -p eosio.token`
   
   * If you remove the entry and later add it again the fanclub is reset... make this decision with care!
   
## Suggested airgrab_json:
RIDL:
```
{
    "account_name": "ridlridlcoin",
    "action_name": "claim",
    "action_parameters": [
       {
          "name": "claimer",
          "type": "account_name"
       }
    ]
}
```   
POOR:
```
{
    "account_name": "poormantoken",
    "action_name": "signup",
    "action_parameters": [
       {
          "name": "owner",
          "type": "account_name"
       },
       {
          "name": "quantity",
          "type": "asset",
          "value": "0.0000 POOR"
       }
    ]
}
```  

## Suggested ico_json
```
{
    "account_name": "eosio.token",
    "action_name": "transfer",
    "action_parameters": [
       {
          "from": "owner",
          "type": "account_name"
       },
       {
          "from": "to",
          "type": "account_name",
          "value": "supercoolico"
       },
       {
          "name": "quantity",
          "type": "asset"
       },
       {
          "name": "memo",
          "type": "string",
          "value": "purchase ico"
       },
    ]
}
``` 
### All these features will be added to the [EOSToolkit](https://eostoolkit.io) to make it even easier!


