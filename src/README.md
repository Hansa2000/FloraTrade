# OLD Version

Here Included the first attempt on the task by a sequence process. Here also I have included the `.csv` file generating Python program [order_csv_generator.py](order_csv_generator.py) and some pre-generated `.csv` files.

There are a few Versions of the program included.
- [Trader. cpp](Trader.cpp): Contains the basic executable program without any advanced algorithms. The complexity of the project also abstracted to obtain the basic functionality. Here a `executed_rep.csv` will not be created.

- [Trader_using_priority_queue.cpp](Trader_using_priority_queue.cpp): Earlier Versions have been updated here with the priority queue algorithm. Here also `executed_rep.csv` will not be created.

- [Trader_using_priority_queue_tradeLogic.cpp](Trader_using_priority_queue_tradeLogic.cpp): Further modified to fulfill all the requirements except the error handling and time calculation. Here for the first time `executed_rep.csv` is created correctly without any rejection and input validations. By changing the name in **17**<sup>th </sup> line we can change the input file to one of the given pre-generated `.csv` files.

`order.csv`

![Screenshot 2023-09-04 023634](https://github.com/Hansa2000/FloraTrade/assets/92619641/705948bd-cff8-40b7-a1c1-b027342f7800)

Created `executed_trades.csv`

![Screenshot 2023-09-04 024647](https://github.com/Hansa2000/FloraTrade/assets/92619641/be07a316-297e-4498-8c88-427fde3f557f)


All the above versions are not capable of providing the expected complete report. To see improvements that have been made click [here](FloraTrade/README.md)
