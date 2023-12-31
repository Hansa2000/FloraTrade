# FloraTrade - Flower Exchange/Trade Model <sub>powered by LSEG</sub>
 
FloraTrade is a flower exchange/trade model powered by the London Stock Exchange Group (LSEG) that facilitates basic trading operations. It allows traders to submit buy and sell orders in the form of CSV files, which are then executed, and an `execution_rep.csv` file is generated to provide details of the executed trades.

## Input Validation
The input `.csv` file should be composed of 5 rows as given below,

<img width="750" alt="1" src="https://github.com/Hansa2000/FloraTrade/assets/92619641/e3c5398f-e327-477c-801f-8f5f45bdb5ea">

## Output Execution Report
The output `.csv` file should be composed of following rows,

<img width="750" alt="2" src="https://github.com/Hansa2000/FloraTrade/assets/92619641/ddf014bc-b1ea-48f4-b08d-4d5cad185d50">

## Procedure
To do the status few rules have to be followed,
- [x] Different Order books for different flower types.
- [x] Order book initialized empty.
- [x] Passive order (an incoming order which goes into the Order book without executing) - status = 'new'.
- [x] Aggressive order (an incoming order which executes) gives two reports consisting of either 'fill', 'fill', or both.
- [x] The buy side of the order book is sorted in the ascending order of the price. 
- [x] The sell side of the order book is sorted in descending order of the price.
- [x] Orders with the same price are ordered in the time priority (Priority sequence).
- [x] Orders will be rejected if,
  * [x] The number of columns of the input file is not 5.
  * [x] Any column is empty.
  * [x] OrderID contains nonnumerical and nonalphabetical characters.
  * [x] OrderID exceeds the 7-character limit.
  * [x] Instrument does not contain one of {`Rose`, `Lavender`, `Lotus`, `Tulip`, `Orchid`}.
  * [x] Side is not either 1 nor 2.
  * [x] Quantity is not a multiple of 10, not an integer or not in between 10 and 1000.
  * [x] price is not double or not positive.
- [x] If rejected a seperate column will be printed with the reason.
- [x] Time taken for each trade is included in a seperate column.


 In the Process we follow a multithreading algorithm to improve the efficiency of the system. 1000 orders at a time will be read and queue for excecution. Meanwhile anouther 1000 will be read. Two `.h` files were created as `process.h` and `util.h` to fascilitate this process. 

 ## Instruction to `run`

There are few versions of code inside the `src` file and using multi threading we have include two versions inside `multi_threading` file and `FloraTrade` file. 

- [src](src/README.md) contain few versions of initial attempt to overcome the above problem. There step by step new features have been introduiced to reach the desired outcome.
- [multi_threading](multi_threading/README.md) Tried out the concept of multi-threading for the first time in C++.
- The newest updated version is in the [FloraTrade](FloraTrade) folder. Further instructions will be included [inside](FloraTrade/README.md).


## Sample Output
The input `order.csv`

![Screenshot 2023-09-04 090658](https://github.com/Hansa2000/FloraTrade/assets/92619641/cc6f320d-cb67-4250-b99c-8044d54a864f)

The Output 'executed_order.csv`

![Screenshot 2023-09-04 090630](https://github.com/Hansa2000/FloraTrade/assets/92619641/07616561-3376-469d-a4a8-9e4df24de2ab)

For input with million orders

https://github.com/Hansa2000/FloraTrade/assets/92619641/728c5713-6920-4ea3-80a9-c3583a39f159

Check it out by running the `.cpp` file [here](FloraTrade/FloraTrade.cpp)

## Contributors

- Hansa Marasinghe [https://github.com/Hansa2000](https://github.com/Hansa2000)
- Dilupa Vinod [https://github.com/D-Vinod](https://github.com/D-Vinod)
