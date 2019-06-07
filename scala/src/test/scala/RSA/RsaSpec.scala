package RSA

import org.scalatest.FunSuite
import org.scalatest.prop.TableDrivenPropertyChecks

class RsaSpec extends FunSuite with TableDrivenPropertyChecks {
    forAll(
        Table(
            "string",
            "aiu",
            "-123",
        )
    ) { string: String =>
        test(s"All String must be digit, so Decimal($string) should be error.") {
            assertThrows[IllegalArgumentException] {
                Decimal(string)
            }
        }
    }

    forAll(
        Table(
            ("left", "right", "answer"),
            (Decimal("123"), Decimal("123"), false),
            (Decimal("0"), Decimal("123"), false),
            (Decimal("0"), -Decimal("0"), false),
            (Decimal("1"), -Decimal("0"), true),
        )
    ) { (left: Decimal, right: Decimal, answer: Boolean) =>
        test(s"Decimal($left) > Decimal($right) == $answer") {
            assert((left > right) == answer)
        }
    }

    forAll(
        Table(
            "decimal",
            Decimal("123"),
            Decimal("0"),
        )
    ) { decimal: Decimal =>
        test(s"-(-$decimal) must be not same instance to $decimal.") {
            assert(!(-(-decimal)).eq(decimal))
        }
    }

    forAll(
        Table(
            ("left", "right", "ans"),
            (Decimal("123"), Decimal("1234"), Decimal("1357")),
            (Decimal("999"), Decimal("9999"), Decimal("10998")),
            (Decimal("998"), Decimal("9999"), Decimal("10997")),
            (-Decimal("998"), -Decimal("9999"), -Decimal("10997")),
            (Decimal("998"), -Decimal("9999"), -Decimal("9001")),
            (-Decimal("998"), Decimal("9999"), Decimal("9001")),
            (Decimal("9999"), Decimal("1"), Decimal("10000")),
        )
    ) { (left: Decimal, right: Decimal, ans: Decimal) =>
        test(
            s"test: (+) -> \n\t $left $right == $ans"
        ) {
            assert(left + right == ans)
        }
    }

    forAll(
        Table(
            ("left", "right", "ans"),
            (Decimal("123"), Decimal("1234"), -Decimal("1111")),
            (Decimal("999"), Decimal("9999"), -Decimal("9000")),
            (Decimal("998"), Decimal("9999"), -Decimal("9001")),
            (-Decimal("998"), -Decimal("9999"), Decimal("9001")),
            (Decimal("998"), -(-(-Decimal("9999"))), Decimal("10997")),
            (-Decimal("998"), Decimal("9999"), -Decimal("10997")),
            (Decimal("100000"), Decimal("1"), Decimal("99999")),
            (Decimal("7015598"), Decimal("1704"), Decimal("7013894")),
            (Decimal("104"), Decimal("10"), Decimal("94")),
            (Decimal("1151"), Decimal("151") * Decimal("7"), Decimal("94")),
            (Decimal("1001"), Decimal("152"), Decimal("849")),
        )
    ) {
        (left: Decimal, right: Decimal, ans: Decimal) =>
            test(s"test: (-) -> \n\t $left - $right == $ans") {
                assert(left - right == ans)
            }
    }

    forAll(
        Table(
            ("left", "right", "ans"),
            (Decimal("123"), Decimal("1234"), Decimal("151782")),
            (Decimal("999"), Decimal("9999"), Decimal("9989001")),
            (-Decimal("998"), -Decimal("9999"), Decimal("9979002")),
            (Decimal("998"), -(-(-Decimal("9999"))), -Decimal("9979002")),
            (-Decimal("998"), Decimal("9999"), -Decimal("9979002")),
            (Decimal("100000"), Decimal("1"), Decimal("100000")),
            (Decimal("10000"), Decimal("0"), Decimal("0")),
        )
    ) {
        (left: Decimal, right: Decimal, ans: Decimal) =>
            test(s"test: (*) -> \n\t $left * $right == $ans") {
                assert(left * right == ans)
            }
    }

    forAll(
        Table(
            ("left", "right", "ans"),
            (Decimal("123"), Decimal("1234"), (Decimal("0"), Decimal("123"))),
            (Decimal("123"), Decimal("123"), (Decimal("1"), Decimal("0"))),
            (Decimal("152151"), Decimal("151"), (Decimal("1007"), Decimal("94"))),
            (Decimal("75017510581"), Decimal("195871"), (Decimal("382994"), Decimal("92807"))),
            (Decimal("10099"), Decimal("7"), (Decimal("1442"), Decimal("5"))),
            (Decimal("10000"), Decimal("47"), (Decimal("212"), Decimal("36"))),
            (Decimal("10000"), Decimal("1000"), (Decimal("10"), Decimal("0"))),
            (Decimal("100"), Decimal("2"), (Decimal("50"), Decimal("0"))),
            (Decimal("10"), Decimal("2"), (Decimal("5"), Decimal("0"))),
        )
    ) {
        (left: Decimal, right: Decimal, ans: (Decimal, Decimal)) =>
            test(s"test: (%) -> \n\t $left % $right == $ans") {
                assert(left % right == ans)
            }
    }
    forAll(
        Table(
            ("target", "exp", "modular", "ans"),
            (Decimal("3"), Decimal("100"), Decimal("7"), Decimal("4")),
            (Decimal("3"), Decimal("10000"), Decimal("7"), Decimal("4")),
            (Decimal("251"), Decimal("15175"), Decimal("7259"), Decimal("6243")),
        )
    ) {
        (target: Decimal, exp: Decimal, modular: Decimal, ans: Decimal) =>
            test(s"test: (modExp) -> \n\t $target modExp($exp)($modular) == $ans") {
                assert(target.modExp(exp)(modular) == ans)
            }
    }

    forAll(
        Table(
            ("a", "b"),
            (Decimal("3"), Decimal("100")),
            (Decimal("3"), Decimal("10000")),
            (Decimal("251"), Decimal("15175")),
        )
    ) {
        (a: Decimal, b: Decimal) =>
            val (x: Decimal, y: Decimal) = Decimal.axPlusby(a, b)
            test(
                s"""test: (axPlusby) ->
                   |    $a * $x + $b * $y = 1""".stripMargin
            ) {
                assert(a * x + b * y == Decimal("1"))
            }
    }

    forAll(
        Table(
            ("e", "phi"),
            (Decimal("3"), Decimal("100")),
            (Decimal("3"), Decimal("10000")),
            (Decimal("251"), Decimal("15175")),
        )
    ) {
        (e: Decimal, phi: Decimal) =>
            test(
                s"""test: (getPrivateKey) ->
                   |    getPrivateKey($e, $phi) > 0""".stripMargin
            ) {
                assert(Decimal.getPrivateKey(e, phi) > Decimal("0"))
            }
    }
}
