package RSA

import scala.collection.mutable

trait Number[A] {
    private[RSA] var sign: Boolean
    private[RSA] var value: IndexedSeq[Int]

    def +(other: A): A

    def -(other: A): A

    def *(other: A): A

    def %(other: A): (A, A)

    def unary_-(): A

    def abs(): A

    def modExp(exp: A)(modular: A): A
}

object Decimal {
    private[RSA] val DIGIT: Int = 100

    def apply(string: String): Decimal = {
        val d = new Decimal()
        require(string forall { x => x.isDigit })
        require(string.length <= Decimal.DIGIT)
        d.value = (0 until Decimal.DIGIT).map {
            x => if (string.length > x) string.reverse(x).toByte - '0'.toByte else 0
        }
        d
    }

    def axPlusby(a: Decimal, b: Decimal): (Decimal, Decimal) = {
        // a and b must be mutually prime.
        val (left: Decimal, right: Decimal) = a % b
        if (right == Decimal("1")) {
            (Decimal("1"), -left)
        } else if (right == Decimal("0")) {
            throw new Exception(
                s"""a and b must be mutually prime.
                   |a = $a
                   |b = $b
                 """.stripMargin)
        } else {
            val (ret_left: Decimal, ret_right: Decimal) = axPlusby(b, right)
            (ret_right, -left * ret_right + ret_left)
        }
    }

    def getPrivateKey(e: Decimal, phi: Decimal): Decimal = {
        val (x: Decimal, y: Decimal) = axPlusby(e, phi)
        if (x < Decimal("0")) {
            phi - (-x % phi)._2
        } else {
            x
        }
    }
}

case class Decimal() extends AnyRef with Number[Decimal] with Ordered[Decimal] {
    private[RSA] var sign: Boolean = true
    private[RSA] var value: IndexedSeq[Int] =
        IndexedSeq.fill(Decimal.DIGIT)(0)

    def printValue(): Unit = {
        println(value.reverseIterator.toIndexedSeq)
    }

    override def +(other: Decimal): Decimal = {
        if (this.sign ^ other.sign) {
            val (bigger: Decimal, smaller: Decimal, isPlusBigger: Boolean) =
                if (this.abs > other.abs) {
                    if (this.sign) {
                        (this, other, true)
                    } else {
                        (this, other, false)
                    }
                } else {
                    if (this.sign) {
                        (other, this, false)
                    } else {
                        (other, this, true)
                    }
                }
            val ret = new Decimal
            val ret_value: mutable.IndexedSeq[Int] =
                mutable.IndexedSeq.fill(Decimal.DIGIT)(0)
            for (index <- 0 until Decimal.DIGIT) {
                val output: Int = bigger.value(index) - smaller.value(index)
                if (output + ret_value(index) >= 0) {
                    ret_value.update(index, ret_value(index) + output)
                } else {
                    ret_value.update(index, ret_value(index) + 10 + output)
                    var searchNoZeroIndex = index
                    while (bigger.value(searchNoZeroIndex + 1) == 0) {
                        searchNoZeroIndex += 1
                    }
                    for (zeroIndex <- index + 1 to searchNoZeroIndex) {
                        ret_value.update(zeroIndex, 9)
                    }
                    ret_value(searchNoZeroIndex + 1) -= 1
                }
            }
            ret.value = ret_value
            if (isPlusBigger) {
                ret
            } else {
                -ret
            }
        } else {
            val ret = new Decimal
            val ret_value: mutable.IndexedSeq[Int] = this.value.zip(other.value)
                .map {
                    pair: (Int, Int) => pair._1 + pair._2
                }.to[mutable.IndexedSeq]
            for (i <- ret_value.indices) {
                if (ret_value(i) > 9) {
                    if (i + 1 == Decimal.DIGIT) {
                        throw new Exception("IndexedSeq's length not enough to calculate.")
                    } else {
                        ret_value.update(
                            i + 1,
                            ret_value(i) / 10 + ret_value(i + 1)
                        )
                        ret_value.update(i, ret_value(i) % 10)
                    }
                }
            }
            ret.value = ret_value.toIndexedSeq
            if (!this.sign && !this.sign) {
                -ret
            } else {
                ret
            }
        }
    }

    override def -(other: Decimal): Decimal = {
        this + (-other)
    }

    override def *(other: Decimal): Decimal = {
        var ret_value: Decimal = new Decimal
        for (i <- 0 until Decimal.DIGIT) {
            val valueTimedOne: IndexedSeq[Int] =
                timeHelper(this.value, other.value(i), i)
            val right = new Decimal
            right.value = valueTimedOne
            ret_value = ret_value + right
        }
        if (this.sign ^ other.sign) {
            -ret_value
        } else {
            ret_value
        }
    }

    private[RSA] def timeHelper(v: IndexedSeq[Int], otherInt: Int, digit: Int): IndexedSeq[Int] = {
        require(0 <= otherInt && otherInt < 10)
        require(digit < Decimal.DIGIT)
        val ret_value_one: mutable.IndexedSeq[Int] =
            mutable.IndexedSeq.fill(Decimal.DIGIT)(0)
        for (i <- 0 until Decimal.DIGIT) {
            val output: Int = v(i) * otherInt
            if (i + digit >= Decimal.DIGIT && output != 0) {
                throw new Exception("Decimal.DIGIT is too small.\n\t in timeHelper")
            } else if (output != 0) {
                ret_value_one.update(i + digit, output)
            }
        }
        for (i <- 0 until Decimal.DIGIT) {
            if (i + 1 >= Decimal.DIGIT && ret_value_one(i) > 9) {
                throw new Exception("Decimal.DIGIT is too small.\n\t in timeHelper")
            } else if (ret_value_one(i) > 9) {
                ret_value_one.update(i + 1, ret_value_one(i) / 10 + ret_value_one(i + 1))
                ret_value_one.update(i, ret_value_one(i) % 10)
            }
        }
        ret_value_one
    }


    override def %(other: Decimal): (Decimal, Decimal) = {
        require(other > Decimal("0"))
        // 0以上でないとややこしいため
        require(this >= Decimal("0"))
        if (this < other) {
            return (Decimal("0"), this.clone())
        }
        val iteratorOfThis = this.value.zipWithIndex.reverseIterator.dropWhile(_._1 == 0)
        var (calTarget: Decimal, pointer: Int) = {
            val head = iteratorOfThis.next()
            (Decimal(head._1.toString), head._2)
        }
        val quotient: mutable.IndexedSeq[Int] =
            mutable.IndexedSeq.fill(Decimal.DIGIT)(0)
        for (p <- (0 to pointer).reverse) {
            if (calTarget >= other) {
                var oneValue = 2
                while (other * Decimal(oneValue.toString) <= calTarget) {
                    oneValue += 1
                }
                oneValue -= 1
                quotient.update(p, oneValue)
                calTarget = calTarget - other * Decimal(oneValue.toString)
            }
            if (iteratorOfThis.hasNext) {
                val n: (Int, Int) = iteratorOfThis.next()
                calTarget = calTarget * Decimal("10") + Decimal(n._1.toString)
            } else {
                val ret_left: Decimal = new Decimal
                ret_left.value = quotient
                return (ret_left, calTarget)
            }
        }
        (other, other)
    }

    override def unary_-(): Decimal = {
        val ret: Decimal = this.clone()
        ret.sign = !ret.sign
        ret
    }

    override def modExp(exp: Decimal)(modular: Decimal): Decimal = {
        val (left: Decimal, right: Decimal) = exp % Decimal("2")
        var ans: Decimal = Decimal("0")
        if (left == Decimal("0")) {
            if (right == Decimal("1")) {
                ans = (this % modular)._2
            } else {
                ans = Decimal("1")
            }
        } else {
            if (right == Decimal("1")) {
                ans = ((this.modExp(exp - Decimal("1"))(modular) * (this % modular)._2) % modular)._2
            } else {
                val half: Decimal = this.modExp(left)(modular)
                ans = ((half * half) % modular)._2
            }
        }
        ans
    }

    override def clone(): Decimal = {
        val ret = new Decimal
        ret.sign = this.sign
        ret.value = this.value.toIterator.to[mutable.IndexedSeq]
        ret
    }

    override def abs(): Decimal = {
        if (this.sign) {
            this
        } else {
            -this
        }
    }

    override def compare(that: Decimal): Int = {
        def compareHelperRecursive(left: Iterator[Int], right: Iterator[Int]): Int = {
            if (left.isEmpty) {
                0
            } else {
                val leftVal = left.next()
                val rightVal = right.next()
                if (leftVal > rightVal) {
                    1
                } else if (leftVal < rightVal) {
                    -1
                } else {
                    compareHelperRecursive(left, right)
                }
            }
        }

        if (this == Decimal("0") && that == Decimal("0")) {
            0
        } else if (this.sign && !that.sign) {
            1
        }
        else if (!this.sign && that.sign) {
            -1
        }
        else {
            compareHelperRecursive(this.value.reverseIterator, that.value.reverseIterator)
        }

    }


    override def equals(obj: Any): Boolean = {
        obj match {
            case other: Decimal =>
                if (value == mutable.IndexedSeq.fill(Decimal.DIGIT)(0) && other.value == mutable.IndexedSeq.fill(Decimal.DIGIT)(0)) {
                    true
                } else {
                    value == other.value && sign == other.sign
                }
            case _ => false
        }
    }

    override def toString: String = {
        val s = if (sign) '+' else '-'
        s +: value.reverseIterator.dropWhile(_ == 0).map(n => (n + '0').toChar).mkString("")
    }
}

