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

constructor TokenERC721(string _name, string _symbol) {
    name = _name;
    symbol = _symbol;
}

/* 查询_owner地址拥有的NFT总个数 */
UNMUTABLE
uint256 balanceOf(address owner) {
    _balances.key = owner;
    return _balances.value;
}

/* 查询tokenId资产的所属地址。 */
UNMUTABLE
address ownerOf(uint256 tokenId) {
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
        // _beforeTokenTransfer(from, to, tokenId);

        // Clear approvals from the previous owner
        _approve(Address("0x0"), tokenId);

        _balances.key = from;
        _balances.value = U256SafeSub(_balances.value, U256(1));

        _balances.key = to;
        _balances.value = U256SafeAdd(_balances.value, U256(1));
   
        _owners.key = tokenId;
        _owners.value = to;

        Transfer(from, to, tokenId);
    }

/* 将_from地址所拥有的tokenId资产转移给to地址。
调用方必须是资产主人或是已被授权的地址，否则会抛出异常。 */
MUTABLE
void transferFrom(address from, address  to, uint256 tokenId) {
    Require(_isApprovedOrOwner(GetSender(), tokenId),
    "ERC721: transfer caller is not owner nor approved");

    _transfer(from, to, tokenId); 
}

/* 把tokenId资产授权给to地址。 */
void _approve(address to, uint256 tokenId) {
    _tokenApprovals.key = tokenId;
    _tokenApprovals.value = to;

    Approval(ownerOf(tokenId), to, tokenId);
}

/* 授权nft给to地址 */
MUTABLE
bool approve(address to, uint256 tokenId) {
    address owner = ownerOf(tokenId);
    Assert(!Equal(to, owner), "Approval to current owner");

    Assert(Equal(GetSender(), owner) || isApprovedForAll(owner, GetSender()),
        "ERC721: approve caller is not owner nor approved for all"
    );

    _approve(to, tokenId);
    return true;
}

/* 判断该nft是否存在 */
bool _exists(uint256 tokenId) {
    _owners.key = tokenId;

    return !Equal(_owners.value, Address("0x0"));
    // return _owners.value != Address("0x0");
}

/* 查询该nft已获授权者地址 */
UNMUTABLE
address getApproved(uint256 tokenId) {
    Assert(_exists(tokenId), "ERC721: approved query for nonexistent token");

    _tokenApprovals.key = tokenId;
    return _tokenApprovals.value;
}

/* Approve `operator` to operate on all of `owner` tokens */
void _setApprovalForAll(
    address owner,
    address operator,
    bool approved
) {
    Assert(!Equal(owner, operator), "ERC721: approve to caller");
    //_operatorApprovals[owner][operator] = approved;
    _operatorApprovals.key = owner;
    _operatorApprovals.value.key = operator;
    _operatorApprovals.value.value = approved;

    ApprovalForAll(owner, operator, approved);
}

MUTABLE
void setApprovalForAll(address operator, bool approved) {
    _setApprovalForAll(GetSender(), operator, approved);
}

UNMUTABLE
bool isApprovedForAll(address owner, address operator) {
    _operatorApprovals.key = owner;
    _operatorApprovals.value.key = operator;
    
    return _operatorApprovals.value.value;
}

bool _isApprovedOrOwner(address spender, uint256 tokenId) {
    Require(_exists(tokenId), "ERC721: operator query for nonexistent token");
    address owner = ownerOf(tokenId);
    
    return (Equal(spender,owner) || Equal(getApproved(tokenId),spender) || isApprovedForAll(owner, spender));
}

// 私有
void  _mint(address to, uint256 tokenId) {
    Require(!Equal(to, Address("0x0")), "ERC721: mint to the zero address");
    Require(!_exists(tokenId), "ERC721: token already minted");

    // _beforeTokenTransfer(address(0), to, tokenId);
    _balances.key = to;
    _balances.value = U256SafeAdd(_balances.value, U256(1));
    _owners.key = tokenId;
    _owners.value = to;

    Transfer(Address("0x0"), to, tokenId);
}

// 私有
void _setTokenURI(uint256 tokenId, string tokenURI) {
    Require(_exists(tokenId), "ERC721URIStorage: URI set of nonexistent token");
    _tokenURIs.key = tokenId;
    _tokenURIs.value = tokenURI;
}

void _burn(uint256 tokenId) {
    address owner = ownerOf(tokenId);

    // Clear approvals
    _approve(Address("0x0"), tokenId);

    _balances.key = owner;
    _balances.value = U256SafeSub(_balances.value, U256(1));
    //delete _owners[tokenId];

    Transfer(owner, Address("0x0"), tokenId);
}

UNMUTABLE
string tokenURI(uint256 tokenId) {
    _tokenURIs.key = tokenId;
    return _tokenURIs.value;
}

UNMUTABLE
string GetTokenName() { return name; }

UNMUTABLE
string GetSymbol() { return symbol; }


MUTABLE
uint256 awardItem(address player, string tokenURI) {
    uint256 _currentId = tokenId;

    _mint(player, tokenId);
    _setTokenURI(tokenId, tokenURI);

    tokenId = U256SafeAdd(tokenId, U256(1));

    return _currentId;
}

_() {}



