# Blockchain-c-
a simple Blockchain with c++
refered to https://github.com/dvf/blockchain

usage:

## 1.build it
    * >> run "g++ *.cpp" in Blockchain-c-/ path, it will create a a.exe in Blockchain-c-/ path.<br>
    * >> run "g++ *.cpp" in Blockchain-c-/user/ path, it will create a a.exe in Blockchain-c-/user/ path.<br>

## 2.run it
    * >> run "./a.exe 5000" in Blockchain-c-/ path, it will startup a node on 127.0.0.1:5000.<br>
    * >> run "./a.exe 5001" in Blockchain-c-/ path, it will startup a node on 127.0.0.1:5001.<br>
    * >> run "./a.exe 127.0.0.1 5000 "new_transaction user1 user2 99"" in Blockchain-c-/user/ path, it will send a transaction info to 127.0.0.1:5000 node.<br>
    * >> run "./a.exe 127.0.0.1 5000 "mine"" in Blockchain-c-/user/ path, it will create a block that have last transaction info in 127.0.0.1:5000 node.<br>
    * >> run "./a.exe 127.0.0.1 5001 "register_nodes 127.0.0.1:5000"" in Blockchain-c-/user/ path, it will let 127.0.0.1:5001 to konw 127.0.0.1:5000 is its neighbour.<br>
    * >> run "./a.exe 127.0.0.1 5001 "consensus"" in Blockchain-c-/user/ path, it will make blocks of 127.0.0.1:5001 are newest as 127.0.0.1:5000.<br>
    
