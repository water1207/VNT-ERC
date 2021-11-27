#include "vntlib.h"

KEY string name, symbol;
KEY uint256 tokenId = U256(0);

KEY mapping(uint256, address) _owners;
KEY mapping(address, uint256) _balances;
KEY mapping(uint256, address) _tokenApprovals;
KEY mapping(address, mapping(address, bool))  _operatorApprovals;
KEY mapping(uint256, string)  _tokenURIs;

EVENT Transfer(indexed address from, indexed address to, uint256 tokenId);
EVENT Approval(indexed address owner, indexed address approved, indexed uint256 tokenId);
EVENT ApprovalForAll(indexed address owner, indexed address operator, bool approved);

void _approve(address to, uint256 tokenId);
bool isApprovedForAll(address owner, address operator);
bool _isApprovedOrOwner(address spender, uint256 tokenId);


void keyihg9lcrn(){
AddKeyInfo( &_balances.value, 5, &_balances, 9, false);
AddKeyInfo( &_balances.value, 5, &_balances.key, 7, false);
AddKeyInfo( &_tokenApprovals.value, 7, &_tokenApprovals, 9, false);
AddKeyInfo( &_tokenApprovals.value, 7, &_tokenApprovals.key, 5, false);
AddKeyInfo( &_operatorApprovals.value.value, 8, &_operatorApprovals, 9, false);
AddKeyInfo( &_operatorApprovals.value.value, 8, &_operatorApprovals.key, 7, false);
AddKeyInfo( &_operatorApprovals.value.value, 8, &_operatorApprovals.value.key, 7, false);
AddKeyInfo( &_tokenURIs.value, 6, &_tokenURIs, 9, false);
AddKeyInfo( &_tokenURIs.value, 6, &_tokenURIs.key, 5, false);
AddKeyInfo( &name, 6, &name, 9, false);
AddKeyInfo( &symbol, 6, &symbol, 9, false);
AddKeyInfo( &tokenId, 5, &tokenId, 9, false);
AddKeyInfo( &_owners.value, 7, &_owners, 9, false);
AddKeyInfo( &_owners.value, 7, &_owners.key, 5, false);
}
constructor TokenERC721(string _name, string _symbol) {
keyihg9lcrn();
InitializeVariables();
    name = _name;
    symbol = _symbol;
}

                                             
UNMUTABLE
uint256 balanceOf(address owner) {
keyihg9lcrn();
    _balances.key = owner;
    return _balances.value;
}

                                           
UNMUTABLE
address ownerOf(uint256 tokenId) {
keyihg9lcrn();
    _owners.key = tokenId;
    return _owners.value;
} 

void _transfer(
        address from,
        address to,
        uint256 tokenId
    ) {
        Require(Equal(ownerOf(tokenId), from), "ERC721: transfer of token that is not own");
        Require(!Equal(to, Address("0x0")), "ERC721: transfer to the zero address");
                                                    

                                                   
        _approve(Address("0x0"), tokenId);

        _balances.key = from;
        _balances.value = U256SafeSub(_balances.value, U256(1));

        _balances.key = to;
        _balances.value = U256SafeAdd(_balances.value, U256(1));
   
        _owners.key = tokenId;
        _owners.value = to;

        Transfer(from, to, tokenId);
    }

                                                               
                                                                                       
MUTABLE
void transferFrom(address from, address  to, uint256 tokenId) {
keyihg9lcrn();
    Require(_isApprovedOrOwner(GetSender(), tokenId),
    "ERC721: transfer caller is not owner nor approved");

    _transfer(from, to, tokenId); 
}

                                          
void _approve(address to, uint256 tokenId) {
    _tokenApprovals.key = tokenId;
    _tokenApprovals.value = to;

    Approval(ownerOf(tokenId), to, tokenId);
}

                          
MUTABLE
bool approve(address to, uint256 tokenId) {
keyihg9lcrn();
    address owner = ownerOf(tokenId);
    Assert(!Equal(to, owner), "Approval to current owner");

    Assert(Equal(GetSender(), owner) || isApprovedForAll(owner, GetSender()),
        "ERC721: approve caller is not owner nor approved for all"
    );

    _approve(to, tokenId);
    return true;
}

                              
bool _exists(uint256 tokenId) {
    _owners.key = tokenId;

    return !Equal(_owners.value, Address("0x0"));
                                               
}

                                       
UNMUTABLE
address getApproved(uint256 tokenId) {
keyihg9lcrn();
    Assert(_exists(tokenId), "ERC721: approved query for nonexistent token");

    _tokenApprovals.key = tokenId;
    return _tokenApprovals.value;
}

                                                            
void _setApprovalForAll(
    address owner,
    address operator,
    bool approved
) {
    Assert(!Equal(owner, operator), "ERC721: approve to caller");
                                                      
    _operatorApprovals.key = owner;
    _operatorApprovals.value.key = operator;
    _operatorApprovals.value.value = approved;

    ApprovalForAll(owner, operator, approved);
}

MUTABLE
void setApprovalForAll(address operator, bool approved) {
keyihg9lcrn();
    _setApprovalForAll(GetSender(), operator, approved);
}

UNMUTABLE
bool isApprovedForAll(address owner, address operator) {
keyihg9lcrn();
    _operatorApprovals.key = owner;
    _operatorApprovals.value.key = operator;
    
    return _operatorApprovals.value.value;
}

bool _isApprovedOrOwner(address spender, uint256 tokenId) {
    Require(_exists(tokenId), "ERC721: operator query for nonexistent token");
    address owner = ownerOf(tokenId);
    
    return (Equal(spender,owner) || Equal(getApproved(tokenId),spender) || isApprovedForAll(owner, spender));
}

          
void  _mint(address to, uint256 tokenId) {
    Require(!Equal(to, Address("0x0")), "ERC721: mint to the zero address");
    Require(!_exists(tokenId), "ERC721: token already minted");

                                                      
    _balances.key = to;
    _balances.value = U256SafeAdd(_balances.value, U256(1));
    _owners.key = tokenId;
    _owners.value = to;

    Transfer(Address("0x0"), to, tokenId);
}

          
void _setTokenURI(uint256 tokenId, string tokenURI) {
    Require(_exists(tokenId), "ERC721URIStorage: URI set of nonexistent token");
    _tokenURIs.key = tokenId;
    _tokenURIs.value = tokenURI;
}

void _burn(uint256 tokenId) {
    address owner = ownerOf(tokenId);

                       
    _approve(Address("0x0"), tokenId);

    _balances.key = owner;
    _balances.value = U256SafeSub(_balances.value, U256(1));
                               

    Transfer(owner, Address("0x0"), tokenId);
}

UNMUTABLE
string tokenURI(uint256 tokenId) {
keyihg9lcrn();
    _tokenURIs.key = tokenId;
    return _tokenURIs.value;
}

UNMUTABLE
string GetTokenName() {
keyihg9lcrn(); return name; }

UNMUTABLE
string GetSymbol() {
keyihg9lcrn(); return symbol; }


MUTABLE
uint256 awardItem(address player, string tokenURI) {
keyihg9lcrn();
    uint256 _currentId = tokenId;

    _mint(player, tokenId);
    _setTokenURI(tokenId, tokenURI);

    tokenId = U256SafeAdd(tokenId, U256(1));

    return _currentId;
}

_() {
keyihg9lcrn();}



