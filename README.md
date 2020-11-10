## Queue Test Plan
----

### Intoduction
This document will describe the process and procedures that will be used to test a sample impementation of a FIFO Queue program. This test is to be used for the interview of Drew Franz for the SDET Engineer role at Nintendo of America, Inc.

### Scope of Testing

Features to be tested:
- Creation of multiple queue classes and handlers
- Destuction of multiple previously created queues
- Adding new bytes to a queue
- Removing bytes off a queue
- Propper use of memory limitations
- Minimum number of queues implemented
- Average number of bytes per queue
- Out of memory exception calling
- Illegal request exception calling

Features not to be tested:
- Infinite number of concurent queues
- Processor performance testing
- Method access limitations
- Security protocol validation

### Testing Approach
Tests will be implemented via a suite of unit tests written in C++, utilizing the GoogleTest testing framework. This will allow the tests to be independent and repeatable. As this is a proof of concept, the number of rounds of tesing will be limited to 1. An exaple test result output file will be provided at `report.xml`

### Deliverables

|File|Submission Time|
|:---|:---|
|QueueTestest.cpp|Initial submission|
|report.xml|Initial submission|

---
### Testing Strategy
A suite of test fixtures will be implemented as unit tests to validate the minimum functionality of the defined functions. These tests will be executed using the GoogleTest framework and reported via an XML file.

#### Assumptions
- All queues average ~80 bytes / queue
- Must support a minimum of 15 queues
- Use no more than `2048 bytes` to implement all queues

#### Exception cases
 - If funcions run out of memory they will call `void on_out_of_memory();`
 - If the caller makes an illegal request, it will call `void on_illegal_operation();`

