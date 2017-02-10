#include <iostream>

#include "Fixture.hpp"

#include "catch.hpp"
#include "interpreter.hpp"

#include "logging.h"

TEST_CASE("Program 3", "Whole programs")
{
  std::string program = R"DELIM(
    { program3.imp }
    VAR
    a b i
    BEGIN
    READ a;
    i := 1;
    WHILE i <= 1000 DO
    b := 25663607 + 41445257;
    a := b + a;
    i := i + 1;
    ENDWHILE
    WRITE a;

    READ a;
    FOR ii FROM 1234567830 DOWNTO 1234567801 DO
    a := a * 1024;
    ENDFOR
    WRITE a;
    WRITE i;

    a := 1999999999 * 1999999997;
    WRITE a;
    END

  )DELIM";

  auto compiled = Fixture().compile(program);

  std::istringstream compiledFile(compiled);
  std::ostringstream stdOut;
  std::istringstream stdIn(
    "1\n"
    "1\n"
  );
  std::ostringstream programOutput;

  auto result = interpret(compiledFile, stdOut, stdIn, programOutput);
  LOG(programOutput.str());

  REQUIRE(result > 0);

  Fixture().checkOutputStr(programOutput, {
    "67108864001",
    "2037035976334486086268445688409378161051468393665936250636140449354381299763336706183397376",
    "1001",
    "3999999992000000003"
  });
}

TEST_CASE("Program 4", "Whole programs")
{
  std::string program = R"DELIM(
    { program4.imp }
    VAR
    b c j t
    BEGIN
    j := 111111 - 111110;
    READ b;
    READ c;
    IF b > 50 THEN
    b := b % 50;
    ELSE
    SKIP;
    ENDIF
    t := b + 1000;
    t := t - 1;
    WRITE t;
    FOR i FROM b TO t DO
    t := i / j;
    c := c + t;
    t := i % i;
    c := c + t;
    ENDFOR
    WRITE t;
    WRITE c;
    END

  )DELIM";

  auto compiled = Fixture().compile(program);

  std::istringstream compiledFile(compiled);
  std::ostringstream stdOut;
  std::istringstream stdIn(
    "52\n"
    "15\n"
  );
  std::ostringstream programOutput;

  auto result = interpret(compiledFile, stdOut, stdIn, programOutput);
  LOG(programOutput.str());

  REQUIRE(result > 0);

  Fixture().checkOutputStr(programOutput, {
    "1001",
    "0",
    "501515"
  });
}


TEST_CASE("Program 5", "Whole programs")
{
  std::string program = R"DELIM(
    { program5.imp }
    VAR
    a b
    BEGIN
    READ a;
    a := 5 + a;
    b := a - 2047;
    a := a % 4;

    IF a < b THEN
    SKIP;
    ELSE
    WRITE a;
    ENDIF

    IF b <= a THEN
    WRITE b;
    ELSE
    SKIP;
    ENDIF
    END
  )DELIM";

  auto compiled = Fixture().compile(program);

  std::istringstream compiledFile(compiled);
  std::ostringstream stdOut;
  std::istringstream stdIn(
    "1020\n"
  );
  std::ostringstream programOutput;

  auto result = interpret(compiledFile, stdOut, stdIn, programOutput);
  LOG(programOutput.str());

  REQUIRE(result > 0);

  Fixture().checkOutputStr(programOutput, {
    "1",
    "0"
  });
}

TEST_CASE("Program 6", "Whole programs")
{
  std::string program = R"DELIM(
    { program6.imp }
    VAR
    fact n
    BEGIN
    READ n;
    fact := 1;
    FOR k FROM 1 TO n DO
    fact := fact * k;
    ENDFOR
    WRITE fact;

    { Liczy n! }
    END


  )DELIM";

  auto compiled = Fixture().compile(program);

  SECTION("Result for 1234")
  {
    std::istringstream compiledFile(compiled);
    std::ostringstream stdOut;
    std::istringstream stdIn(
      "123\n"
    );
    std::ostringstream programOutput;

    auto result = interpret(compiledFile, stdOut, stdIn, programOutput);
    LOG(programOutput.str());

    REQUIRE(result > 0);

    Fixture().checkOutputStr(programOutput, {
      "12146304367025329675766243241881295855454217088483382315328918161829235892362167668831156960612640202170735835221294047782591091570411651472186029519906261646730733907419814952960000000000000000000000000000"
    });
  }
}

TEST_CASE("Program 7", "Whole programs")
{
  std::string program = R"DELIM(
    { program7.imp }
    { Czas wykonywania tego programu nie powinien istotnie roznic sie
      od czasu wykonywania program6.imp (dla tych samych n). }
      VAR
      fact n
      BEGIN
      READ n;
      fact := 1;
      FOR k FROM 1 TO n DO
      fact := k * fact;
      ENDFOR
      WRITE fact;

      { Liczy n! }
      END

    )DELIM";

    auto compiled = Fixture().compile(program);

    SECTION("Result for 1234")
    {
      std::istringstream compiledFile(compiled);
      std::ostringstream stdOut;
      std::istringstream stdIn(
        "123\n"
      );
      std::ostringstream programOutput;

      auto result = interpret(compiledFile, stdOut, stdIn, programOutput);
      LOG(programOutput.str());

      REQUIRE(result > 0);

      Fixture().checkOutputStr(programOutput, {
        "12146304367025329675766243241881295855454217088483382315328918161829235892362167668831156960612640202170735835221294047782591091570411651472186029519906261646730733907419814952960000000000000000000000000000"
      });
    }
  }

  TEST_CASE("Program 8", "Whole programs")
  {
    std::string program = R"DELIM(
      { program8.imp }
      VAR
      a b c
      BEGIN
      READ a;
      READ b;
      READ c;

      FOR i FROM 9 DOWNTO 0 DO
      FOR j FROM 0 TO i DO
      FOR k FROM 0 TO j DO
      a := a + k;
      c := k * j;
      c := c + i;
      b := b + c;
      ENDFOR
      ENDFOR
      ENDFOR

      WRITE a;
      WRITE b;
      WRITE c;
      END
    )DELIM";

    auto compiled = Fixture().compile(program);

    SECTION("123")
    {
      std::istringstream compiledFile(compiled);
      std::ostringstream stdOut;
      std::istringstream stdIn(
        "1\n"
        "2\n"
        "3\n"
      );
      std::ostringstream programOutput;

      auto result = interpret(compiledFile, stdOut, stdIn, programOutput);
      LOG(programOutput.str());

      REQUIRE(result > 0);

      Fixture().checkOutputStr(programOutput, {
        "496",
        "4358",
        "0"
      });
    }
  }


  TEST_CASE("Program 9", "Whole programs")
  {
    std::string program = R"DELIM(
      { program9.imp }
      VAR
      n j ta[25] tb[25] tc[25]
      BEGIN
      n := 25 - 1;
      tc[0] := n;
      tc[n] := n - n;

      FOR i FROM tc[0] DOWNTO tc[n] DO
      ta[i] := i;
      tb[i] := n - i;
      ENDFOR

      FOR i FROM tc[n] TO tc[0] DO
      tc[i] := ta[i] * tb[i];
      ENDFOR

      FOR i FROM 0 TO n DO
      WRITE tc[i];
      ENDFOR
      END
    )DELIM";

    auto compiled = Fixture().compile(program);

    SECTION("123")
    {
      std::istringstream compiledFile(compiled);
      std::ostringstream stdOut;
      std::istringstream stdIn;

      std::ostringstream programOutput;

      auto result = interpret(compiledFile, stdOut, stdIn, programOutput);
      LOG(programOutput.str());

      REQUIRE(result > 0);

      Fixture().checkOutputStr(programOutput, {
        "0",
        "23",
        "44",
        "63",
        "80",
        "95",
        "108",
        "119",
        "128",
        "135",
        "140",
        "143",
        "144",
        "143",
        "140",
        "135",
        "128",
        "119",
        "108",
        "95",
        "80",
        "63",
        "44",
        "23",
        "0"
      });
    }
  }

  TEST_CASE("Program 10", "Whole programs")
  {
    std::string program = R"DELIM(
      { program10.imp }
      VAR
      a b c t[7] d e f g h i j tab[6]
      BEGIN
      WRITE 0;
      WRITE 1;
      WRITE 2;
      WRITE 10;
      WRITE 100;
      WRITE 10000;
      WRITE 1234567890;

      a := 1234566543;
      b := 677777177;
      c := 15;
      t[2] := 555555555;
      d := 8888;
      tab[4] := 11;
      t[0] := 999;
      e := 1111111111;
      tab[0] := 7777;
      f := 2048;
      g := 123;
      t[3] := t[0];
      tab[5] := a;
      tab[5] := tab[0] / tab[4];
      t[5] := tab[0];

      READ h;
      i := 1;
      j := h + c;

      WRITE j; { j = h + 15 }
      WRITE c; { c = 15 }
      WRITE t[3]; { 999 }
      WRITE t[2]; { 555555555 }
      WRITE t[5]; { 7777 }
      WRITE t[0]; { 999 }
      WRITE tab[4]; { 11 }
      WRITE tab[5]; { 707 }
      WRITE tab[0]; { 7777 }

      END
    )DELIM";

    auto compiled = Fixture().compile(program);

    SECTION("123")
    {
      std::istringstream compiledFile(compiled);
      std::ostringstream stdOut;
      std::istringstream stdIn("12345678900\n");

      std::ostringstream programOutput;

      auto result = interpret(compiledFile, stdOut, stdIn, programOutput);
      LOG(programOutput.str());

      REQUIRE(result > 0);

      Fixture().checkOutputStr(programOutput, {
        "0",
        "1",
        "2",
        "10",
        "100",
        "10000",
        "1234567890",
        "12345678915",
        "15",
        "999",
        "555555555",
        "7777",
        "999",
        "11",
        "707",
        "7777",
      });
    }
  }

  TEST_CASE("Program 11", "Whole programs")
  {
    std::string program = R"DELIM(
      { program11.imp }
      VAR
      a aa b c d e f g
      BEGIN
      READ a;
      READ aa;
      READ b;

      c := a + 10;
      d := b - 5;
      e := 1;
      f := d + e;
      g := 1111111111;

      WRITE g;
      WRITE a;
      WRITE aa;
      WRITE b;
      WRITE c;
      WRITE d;
      WRITE e;
      WRITE f;
      WRITE g;

      IF a > b THEN
      e := a + c;
      IF a > aa THEN
      f := e * aa;
      ELSE
      f := e * a;
      ENDIF
      ELSE
      e := b + c;
      IF aa > a THEN
      f := e / a;
      ELSE
      f := e / aa;
      ENDIF
      ENDIF

      WRITE a;
      WRITE aa;
      WRITE b;
      WRITE c;
      WRITE d;
      WRITE e;
      WRITE f;

      END
    )DELIM";

    auto compiled = Fixture().compile(program);

    SECTION("123")
    {
      std::istringstream compiledFile(compiled);
      std::ostringstream stdOut;
      std::istringstream stdIn(
        "12345678900\n"
        "123\n"
        "4321\n"

      );

      std::ostringstream programOutput;

      auto result = interpret(compiledFile, stdOut, stdIn, programOutput);
      LOG(programOutput.str());

      REQUIRE(result > 0);

      Fixture().checkOutputStr(programOutput, {
        "1111111111",
        "12345678900",
        "123",
        "4321",
        "12345678910",
        "4316",
        "1",
        "4317",
        "1111111111",
        "12345678900",
        "123",
        "4321",
        "12345678910",
        "4316",
        "24691357810",
        "3037037010630",
      });
    }
  }

  TEST_CASE("Program 12", "Whole programs")
  {
    std::string program = R"DELIM(
      { program12.imp }
      VAR
      a aa[5] b c d
      BEGIN
      READ a;
      READ b;
      READ c;
      READ d;

      aa[0] := a / 0;
      aa[1] := 0 / a;
      aa[2] := 0 / 0;
      aa[3] := a / 1;
      aa[4] := 1 / a;

      WRITE aa[0]; { 0 }
      WRITE aa[1]; { 0 }
      WRITE aa[2]; { 0 }
      WRITE aa[3]; { a }
      WRITE aa[4]; { a == 1 ? 1 : 0 }

      WRITE 1111111111;

      aa[0] := a * 0;
      aa[1] := 0 * a;
      aa[2] := a * 1;
      aa[3] := 1 * a;

      WRITE aa[0]; { 0 }
      WRITE aa[1]; { 0 }
      WRITE aa[2]; { a }
      WRITE aa[3]; { a }

      WRITE 1111111111;

      aa[0] := a % 0;
      aa[1] := 0 % a;
      aa[2] := a % 1;
      aa[3] := 1 % a;

      WRITE aa[0]; { 0 }
      WRITE aa[1]; { 0 }
      WRITE aa[2]; { 0 }
      WRITE aa[3]; { a < 2 ? 0 : 1 }

      WRITE 1111111111;

      aa[0] := a + 1;
      aa[1] := a - a;
      aa[2] := a - aa[0];
      aa[3] := aa[0] - a;

      WRITE aa[0]; { a + 1 }
      WRITE aa[1]; { 0 }
      WRITE aa[2]; { 0 }
      WRITE aa[3]; { 1 }

      WRITE 1111111111;

      END


    )DELIM";

    auto compiled = Fixture().compile(program);

    SECTION("123")
    {
      std::istringstream compiledFile(compiled);
      std::ostringstream stdOut;
      std::istringstream stdIn(
        "3\n"
        "0\n"
        "0\n"
        "0\n"
      );

      std::ostringstream programOutput;

      auto result = interpret(compiledFile, stdOut, stdIn, programOutput);
      LOG(programOutput.str());

      REQUIRE(result > 0);
      Fixture().checkOutputStr(programOutput, {
        "0",
        "0",
        "0",
        "3",
        "0",
        "1111111111",
        "0",
        "0",
        "3",
        "3",
        "1111111111",
        "0",
        "0",
        "0",
        "1",
        "1111111111",
        "4",
        "0",
        "0",
        "1",
        "1111111111"
      });
    }
  }

  TEST_CASE("Program 13", "Whole programs")
  {
    std::string program = R"DELIM(
      { program13.imp }
      VAR
      aa bb cc x[4] dd ee
      BEGIN
      READ x[0];
      READ x[1];
      READ x[2];
      READ x[3];

      aa := x[0] + x[0];
      bb := x[1] - x[1];
      cc := x[2] * x[2];
      dd := x[3] / x[3];
      ee := x[3] % x[3];

      WRITE 999999999;

      WRITE aa; { x[0] + x[0] }
      WRITE bb; { 0 }
      WRITE cc; { x[2]^2 }
      WRITE dd; { x[3] == 0 > 0 : 1 }
      WRITE ee; { 0 }



      aa := x[0] + x[1];
      bb := x[1] - x[0];
      cc := x[2] * x[3];
      dd := x[3] / x[2];
      ee := x[0] % x[1];

      WRITE 999999999;

      WRITE aa;
      WRITE bb;
      WRITE cc;
      WRITE dd;
      WRITE ee;
      END
    )DELIM";

    auto compiled = Fixture().compile(program);

    SECTION("123")
    {
      std::istringstream compiledFile(compiled);
      std::ostringstream stdOut;
      std::istringstream stdIn(
        "2\n"
        "3\n"
        "4\n"
        "5\n"
      );

      std::ostringstream programOutput;

      auto result = interpret(compiledFile, stdOut, stdIn, programOutput);
      LOG(programOutput.str());

      REQUIRE(result > 0);
      Fixture().checkOutputStr(programOutput, {
        "999999999",
        "4",
        "0",
        "16",
        "1",
        "0",
        "999999999",
        "5",
        "1",
        "20",
        "1",
        "2"
      });
    }
  }

  TEST_CASE("Program 14", "Whole programs")
  {
    std::string program = R"DELIM(
      { progrm14.imp }
      VAR
      tab[22] x q w j k n m
      BEGIN
      n := 23;
      m := n - 2;
      q := 5;
      w := 1;

      {generowanie nieposortowanej tablicy}
      FOR i FROM 0 TO m DO
      w := w * q;
      w := w % n;
      tab[i] := w;
      ENDFOR

      {wypisywanie nieposortowanej tablicy}
      FOR i FROM 0 TO m DO
      WRITE tab[i];
      ENDFOR

      WRITE 1111111111;

      {sortowanie}
      FOR i FROM 1 TO m DO
      x := tab[i];
      j := i;
      WHILE j > 0 DO
      k := j - 1;
      IF tab[k] > x THEN
      tab[j] := tab[k];
      j := j - 1;
      ELSE
      k := j;
      j := 0;
      ENDIF
      ENDWHILE
      tab[k] := x;
      ENDFOR

      {wypisywanie posortowanej tablicy}
      FOR i FROM 0 TO m DO
      WRITE tab[i];
      ENDFOR
      END



    )DELIM";

    auto compiled = Fixture().compile(program);

    SECTION("123")
    {
      std::istringstream compiledFile(compiled);
      std::ostringstream stdOut;
      std::istringstream stdIn;

      std::ostringstream programOutput;

      auto result = interpret(compiledFile, stdOut, stdIn, programOutput);
      LOG(programOutput.str());

      REQUIRE(result > 0);
      Fixture().checkOutputStr(programOutput, {
        "5",
        "2",
        "10",
        "4",
        "20",
        "8",
        "17",
        "16",
        "11",
        "9",
        "22",
        "18",
        "21",
        "13",
        "19",
        "3",
        "15",
        "6",
        "7",
        "12",
        "14",
        "1",
        "1111111111",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "10",
        "11",
        "12",
        "13",
        "14",
        "15",
        "16",
        "17",
        "18",
        "19",
        "20",
        "21",
        "22"
      });
    }
  }
