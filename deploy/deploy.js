var fs = require('fs'); 
var Vnt = require('vnt');
var vntkit = require('vnt-kit');
var TX = require('ethereumjs-tx');
var vnt = new Vnt();

var privider = "http://101.37.164.86:8880";
var privider2 = 'http://47.102.157.204:8880';
vnt.setProvider(new vnt.providers.HttpProvider(privider));

var ksDir = "./keystore/"
var kFile1 = "key1"
var pass1 = ""
var kFile2 ="key2"
var pass2 = ""
var kFile3 ="key3"
var pass3 = ""

function openAccount(file, passwd) {
    var content = fs.readFileSync(file).toString("utf-8")
    return vntkit.account.decrypt(content, passwd, false)
}

var account1 = openAccount(ksDir + kFile1, pass1)
var account2 = openAccount(ksDir + kFile2, pass2)
var account3 = openAccount(ksDir + kFile3, pass3)

var codeFile = "../output/TokenERC721.compress"
var abiFile = "../output/TokenERC721.abi"
//读取abi数据
var wasmabi = fs.readFileSync(abiFile)
//将abi数据解析成json结构
var abi = JSON.parse(wasmabi.toString("utf-8"))
// 使用abi生成合约实例
var contract = vnt.core.contract(abi)

var contractAddr = '0xd501764ececce39a5d9b6fa78553631b2c85915e'

function getTransactionReceipt(tx, cb) {
    var receipt = vnt.core.getTransactionReceipt(tx);
    if (!receipt) {
      setTimeout(function() {
        getTransactionReceipt(tx, cb)
      }, 1000);
    } else {
      cb(receipt)
    }
}

function sendTransaction(account, data){
    var nonce = vnt.core.getTransactionCount(account.address);

    var options = {
        to: contractAddr,
        nonce: vnt.toHex(nonce),
        gasPrice: vnt.toHex(30000000000000),
        gasLimit: vnt.toHex(4000000),
        value: '0x00',
        data: data,
        chainId: 2
    }

    var tx = new TX(options);
    tx.sign(new Buffer(account.privateKey.substring(2,), 'hex'));
      
    var serializedTx = tx.serialize();
    vnt.core.sendRawTransaction(
      '0x' + serializedTx.toString('hex'), function(err, txHash) {
        if (err) {
          console.log('err happened: ', err)
        } else {
          getTransactionReceipt(txHash, function(receipt) {
            console.log("tx receipt: ", receipt)
          })
          console.log('Tx Successful')
        }
      });
}

function pureTransaction(funName, val) {
    if(val === null)
        var data = contract.packFunctionData(funName, [])
    else
        var data = contract.packFunctionData(funName, [val])

    // 生成交易的结构体，指定to, data等字段
    var options = {
        to: contractAddr,
        data: data
    }
    // 使用vnt.core.call方法进行查询，不会发起交易
    var result = vnt.core.call(options)
    // 对结果进行解码，得出结果
    console.log(contract.unPackOutput(funName, result).toString())
}

function deployWasmContract() {
    // var nonce = vnt.core.getTransactionCount(account1.address);
    // 通过abi与代码路径初始化合约
    var contract = vnt.core.contract(abi).codeFile(codeFile)
    // 生成合约创建的数据
    var data = contract.packContructorData("WontonData", "WD")
    // 预估一个gas值
    var gas = vnt.core.estimateGas({data: data});

    // 获取账户1的下一个nonce值
    var nonce = vnt.core.getTransactionCount(account1.address);

    // 生成交易的结构体，指定nonce, gasPirce, gasLimit, value, data等字段
    var options = {
       nonce: vnt.toHex(nonce),
       gasPrice: vnt.toHex(25000000000000),
       gasLimit: gas,
       value: '0x00',
       data: data,
       chainId: 2  //这里必须指定chainId，即你所连接的node的chainId，否则交易签名将出错
    }

    // 生成交易
    var tx = new TX(options);
    // 使用之前准备好的私钥，对交易签名
    tx.sign(new Buffer(account1.privateKey.substring(2,), "hex"));

    // 将交易数据进行序列化
    var serializedTx = tx.serialize();

    vnt.core.sendRawTransaction('0x' + serializedTx.toString('hex'), function(err, txHash){
        if (err) {
            console.log("err happened: ", err)
        } else {
            // 打印交易的hash
            console.log("transaction hash: ", txHash);
            getTransactionReceipt(txHash, function(receipt) {
                // console.log("tx receipt: ", receipt)
                // console.log("tx status: ", receipt.status)
                console.log("contract address: ", receipt.contractAddress)
            })
        }
    })
}

function GetTokenName() {
    pureTransaction("GetTokenName", null);
}

function getOwner(tokenId) {
    pureTransaction("ownerOf", tokenId)
}

function awardItem(account, uri) {
    // 生成合约调用所用的data。这个data是将awardItem方法的签名和参数加包
    var data = contract.packFunctionData("awardItem", [account.address, uri]);

    sendTransaction(account, data);
}

function approve(account, to, tokenId) {
    var data = contract.packFunctionData("approve", [to, tokenId]);

    sendTransaction(account, data);
}

function getApproved(tokenId) {
    pureTransaction("getApproved", tokenId);
}

function transferFrom(account, from, to, tokenId) {
    var data = contract.packFunctionData("transferFrom", [from, to, tokenId]);

    sendTransaction(account, data);
}

function demo() {
    approve(account2.address, 2);
}

// deployWasmContract();
// GetTokenName();
// awardItem(account1, "https://postimg.cc/nMDnkwSF");
getOwner(2);
// demo()
// getApproved(2);
// transferFrom(account2, account1.address, account3.address, 2);

