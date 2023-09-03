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

There are Two versions of code inside the `src` file and `multi_threading` file.Newest Version is in the [a relative link](path%20with%20spaces/other_file.md) is the newest updated version.
