To run this program, use one of the files located in input as argv[1] or create a file with the format:
<Root> <#of children> <list of children>
<Child 1> <#of children> <list of children>
<...> subsequent children / grand children

Note: The name of each process must be one character long. To avoid overloading the resources, ulimit was set to 25 processes while testing.
