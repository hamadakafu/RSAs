import RSA.Decimal

object Hello {
    def main(args: Array[String]): Unit = {
        val p: Decimal = Decimal("100000000000000003")
        val q: Decimal = Decimal("100000000000000013")
        val e: Decimal = Decimal("170141183460469231731687303715884105727")

        val phi: Decimal = (p - Decimal("1")) * (q - Decimal("1"))
        val d: Decimal = Decimal.getPrivateKey(e, phi)
        val m: Decimal = Decimal("170572")
        println(m)
        val encodeM: Decimal = m.modExp(e)(p * q)
        println(encodeM)
        val decodeM: Decimal = encodeM.modExp(d)(p * q)
        println(decodeM)
    }
}