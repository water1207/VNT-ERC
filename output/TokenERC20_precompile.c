#include "vntlib.h"

                     
                                 
KEY string name, symbol;
KEY uint256 decimals = U256(8), totalSupply;

                                           
KEY mapping(address, uint256) balanceOf;
KEY mapping(address, mapping(address, uint256)) allowance;

                                                                               
EVENT Transfer(indexed address from, indexed address to, uint256 value);

                                                                               
EVENT Approval(indexed address owner, indexed address spender, uint256 value);

                                                   
                                                    

    
                        
   
                                                                         
            
   

void keyam4sbsqq(){
AddKeyInfo( &name, 6, &name, 9, false);
AddKeyInfo( &symbol, 6, &symbol, 9, false);
AddKeyInfo( &decimals, 5, &decimals, 9, false);
AddKeyInfo( &totalSupply, 5, &totalSupply, 9, false);
AddKeyInfo( &balanceOf.value, 5, &balanceOf, 9, false);
AddKeyInfo( &balanceOf.value, 5, &balanceOf.key, 7, false);
AddKeyInfo( &allowance.value.value, 5, &allowance, 9, false);
AddKeyInfo( &allowance.value.value, 5, &allowance.key, 7, false);
AddKeyInfo( &allowance.value.value, 5, &allowance.value.key, 7, false);
}
constructor TokenERC20(uint256 initialSupply, string tokenName,
                       string tokenSymbol)
{
keyam4sbsqq();
InitializeVariables();
  totalSupply = U256SafeMul(
      initialSupply,
      U256_Pow(U256(10),
               decimals));                                                
  balanceOf.key = GetSender();
  balanceOf.value = totalSupply;                                        
  name = tokenName;                                                   
  symbol = tokenSymbol;                                                 
}

    
                                                          
   
void _transfer(address _from, address _to, uint256 _value)
{
                                                         
  Assert(!Equal(_to, Address("0x0")), "empty to address");
                                    
  balanceOf.key = _from;
  Assert(U256_Cmp(balanceOf.value, _value) != -1,
         "sender does not have enough token");
                         
  balanceOf.key = _to;
  Assert(U256_Cmp(U256_Add(balanceOf.value, _value), balanceOf.value) != -1,
         "overflows");
                                              
                            
                                              
                          
                                            
                                                                         
                              
  balanceOf.key = _from;
  balanceOf.value = U256SafeSub(balanceOf.value, _value);
                                   
  balanceOf.key = _to;
  balanceOf.value = U256SafeAdd(balanceOf.value, _value);
  Transfer(_from, _to, _value);
                                                                             
                       
                                                                    
}

    
                   
   
                                                   
   
                                           
                                    
   
MUTABLE
bool transfer(address _to, uint256 _value)
{
keyam4sbsqq();
  _transfer(GetSender(), _to, _value);
  return true;
}

    
                                      
   
                                                      
   
                                          
                                           
                                    
   
MUTABLE
bool transferFrom(address _from, address _to, uint256 _value)
{
keyam4sbsqq();
  allowance.key = _from;
  allowance.value.key = GetSender();
  Assert(U256_Cmp(_value, allowance.value.value) != 1,
         "sender does not have enought token");                    
  allowance.key = _from;
  allowance.value.key = GetSender();
  allowance.value.value = U256SafeSub(allowance.value.value, _value);
  _transfer(_from, _to, _value);
  return true;
}

    
                                   
   
                                                                          
   
                                                   
                                               
   
MUTABLE
bool approve(address _spender, uint256 _value)
{
keyam4sbsqq();
  allowance.key = GetSender();
  allowance.value.key = _spender;
  allowance.value.value = _value;
  Approval(GetSender(), _spender, _value);
  return true;
}

UNMUTABLE
uint256 GetAmount(address addr)
{
keyam4sbsqq();
  balanceOf.key = addr;
  return balanceOf.value;
}

UNMUTABLE
string GetTokenName() {
keyam4sbsqq(); return name; }

UNMUTABLE
string GetSymbol() {
keyam4sbsqq(); return symbol; }

UNMUTABLE
uint256 GetTotalSupply() {
keyam4sbsqq(); return totalSupply; }

UNMUTABLE
uint256 GetDecimals() {
keyam4sbsqq(); return decimals; }

_() {
keyam4sbsqq();}
