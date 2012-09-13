  string print(map<string,Form*> in)
  {
    string out = "{\n";
    for(map<string,Form*>::iterator i = in.begin(); i != in.end(); i++)
    {
      out += "\t" + i->first + " : " + preprint(i->second) + "\n";
    }
    return out + "}\n";
  }
  
  void testAppend(Form* first, Form* second)
  {
    cout << "Input:\n" << first << ", " << second << endl;
    cout << "Output:\n" << append(first,second) << endl;
  }
  
  void runTest_append()
  {
    cout << "Testing: (APPEND)" << endl;
    testAppend(readString("(derp)"),readString("(derp herp)"));
    testAppend(readString("(derp herp)"),readString("(durp hurp)"));
    testAppend(readString("(derp)"),readString("((derp herp))"));
  }
  
  void testEditForm(Form* code, map<string,Form*> replacements)
  {
    cout << "Original form:\n" << code << endl;
    cout << "Replacements:\n" << print(replacements) << endl;
    cout << "Final form:\n" << editForm(code,replacements) << endl;
  }
  
  void runTest_editForm()
  {
    cout << "Testing: Form Editing" << endl;
    map<string,Form*> replacements;
    replacements["A"] = readString("i32");
    replacements["B"] = readString("(sequence char)");
    replacements["C"] = readString("(matrix long)");
    testEditForm(readString("a"),replacements);
    cin.get();
    testEditForm(readString("A"),replacements);
    cin.get();
    testEditForm(readString("(A B)"),replacements);
    cin.get();
    testEditForm(readString("(sample A (derp B) (herp C hurp))"),replacements);
    testEditForm(readString("(inline i64 ((in (sequence Type))) (ret 1))"),replacements);
    cin.get();
  }
  
  void testMakeType(Form* in)
  {
    cout << "Form: " << in << endl;
    cout << "LLVM Code: " << makeType(in) << endl;
  }
  
  void runTest_makeType()
  {
    cout << "Testing: makeType" << endl;
    testMakeType(readString("(type Integer i46)"));
  }
  
  void testMakeStructure(Form* in)
  {
    cout << "Form: " << in << endl;
    cout << "LLVM Code: " << makeStructure(in) << endl;
  }
  
  void runTest_makeStructure()
  {
    cout << "Testing: makeStructure" << endl;
    testMakeStructure(readString("(structure point (x i64) (y i64))"));
  }
  
  void analyzeGeneric(Generic in)
  {
    cout << "   Generic type: " << (in.id ? "Function" : "Structure") << endl;
    cout << "   Generic code: " << in.code << endl;
    cout << "   Generic arguments: " << endl;
    unsigned long i = 0;
    for(; i < in.arguments.size(); i++)
    {
      cout << "      " << in.arguments[i] << " ";
    }
    cout << " \n   Generic specializations: " << endl;
    for(i = 0; i < in.specializations.size(); i++)
    {
      cout << "      " << in.specializations[i] << " ";
    }
    cout << "   Generic methods: " << endl;
    for(i = 0; i < in.methods.size(); i++)
    {
      cout << "      " << preprint(in.methods[i]) << " ";
    }
    cout << endl;
  }
  
  void testMakeGeneric(Form* in)
  {
    cout << "Form: " << in << endl;
    Generic out = makeGeneric(in);
    analyzeGeneric(out);
  }
  
  void runTest_makeGeneric()
  {
    cout << "Testing: makeGeneric" << endl;
    testMakeGeneric(readString("(generic structure derp (a) (element a))"));
    testMakeGeneric(readString("(generic function durp (a b c) (function a (x y z) ...))"));
    testMakeGeneric(readString("(generic structure sequence (Type) (length i64) (data (pointer Type)))"));
    testMakeGeneric(readString("(generic method length of sequence (inline i64 ((in (sequence Type))) 10))"));
  }
  
  void testPrintTypeSignature(Form* in)
  {
    cout << "Form: " << in << endl;
    cout << "Type Signature: " << printTypeSignature(in) << endl;
  }
  
  void runTest_printTypeSignature()
  {
    cout << "Testing: Printing Type Signatures" << endl;
    testPrintTypeSignature(readString("i32"));
    testPrintTypeSignature(readString("long"));
    testPrintTypeSignature(readString("float"));
    testPrintTypeSignature(readString("Integer"));
    testPrintTypeSignature(readString("point"));
    testPrintTypeSignature(readString("(derp i32)"));
    testPrintTypeSignature(readString("(derp i32*)"));
    testPrintTypeSignature(readString("(sequence char)"));
    cout << "Code Stack: " << endl;
    for(unsigned long i = 0; i < CodeStack.size(); i++)
    {
      cout << CodeStack[i] << endl;
    }
  }
  
  void runTests()
  {
    testmode = true;
    /*Form* test = readString("(a)");
    for(unsigned long i = 0; i < length(test); i++)
    {
      cout << "Element " << i << ": " << nth(test,i) << endl;
    }*/
    //runTest_append();
    //runTest_editForm();
    //runTest_makeType();
    //runTest_makeStructure();
    //runTest_makeGeneric();
    //runTest_printTypeSignature();
    string out;
    string tests[] =
    {
      //Testing the language Core
      "10",
      "(add 10 20)",
      "(icmp eq 1 2)",
      "(icmp leq 10 100)",
      //Testing function definition
      "(function test i64 ()\
        10)",
      "(function derp i64 ((n i64))\
        10)",
      "(function test bool ((n i64) (m i64))\
        (icmp eq n m))",
      "(function test2 bool ()\
        (icmp eq 2 2))",      //Testing Function calling
      "(test)",
      "(test 2 3)",
      "(test2)",
      /*"(recursive fib i64 ((n i64))\
        (flow (icmp ult 2 n)\
          n\
            (add (fib (sub n 1))\
                 (fib (sub n 2)))))",*/
      "\0"
    };
    for(unsigned long i = 0; tests[i] != "\0"; i++)
    {
      out += "Test #" + to_string(i+1) + ":\n> " + tests[i] + "\n\n";
      out += emitCode(readString(tests[i])) + "\n\n";
      cout << out << endl;
      cin.get();
    }
  }