import Vector3D
import unittest

class Vector3DTests(unittest.TestCase):
    '''
    Test cases for Vector3D class
    '''

    def setUp(self):
        '''
        Common fixtures to each test.  This function run before each test
        '''
        self.a = Vector3D.Vector3D(1,2,3)
        self.a2 = Vector3D.Vector3D(1,2,3)
        self.b = Vector3D.Vector3D(5,4,6,7,8,9)
        self.c = Vector3D.Vector3D(20,100,244)

    def test_constructor(self):
        self.assertEqual(self.a.x, 1)
        self.assertEqual(self.a.y, 2)
        self.assertEqual(self.a.z, 3)
        self.assertEqual(self.b.x, 5)
        self.assertEqual(self.b.y, 4)
        self.assertEqual(self.b.z, 6)
        self.assertEqual(self.b.xPos, 7)
        self.assertEqual(self.b.yPos, 8)
        self.assertEqual(self.b.zPos, 9)

    def test_eq(self):
        self.assertEqual(self.a,self.a2)
        self.assertNotEqual(self.a,self.b)
        self.assertEqual(self.a, self.a)

    def test_add(self):
        self.a += self.b + self.c
        d = Vector3D.Vector3D(26,106,253)

        self.assertEqual(self.a, d)

    def test_mult(self):
        self.b = self.a * 5
        a5 = Vector3D.Vector3D(5,10,15)
        self.assertEqual(self.b, a5)

        self.a *= 5
        self.assertEqual(self.a, a5)

    def test_length(self):
        self.assertEquals(Vector3D.Vector3D(4,0,3).length(), 5)

    def test_translate(self):
        self.a.translate(3,4,5)
        correct = Vector3D.Vector3D(1,2,3,3,4,5)
        self.assertEquals(self.a, correct)

    def test_dotProd(self):
        a = Vector3D.Vector3D(1,2,3)
        b = Vector3D.Vector3D(5,7,9)
        self.assertEquals(a.dotProd(b), 46)
        # perpindicular
        a = Vector3D.Vector3D(1,0,0);
        b = Vector3D.Vector3D(0,1,0);
        self.assertEquals(a.dotProd(b), 0);

        # parallel
        a = Vector3D.Vector3D(1,2,3);
        self.assertEquals(a.dotProd(a), 14);

    def test_crossProd(self):
        a = Vector3D.Vector3D(1,2,3);
        b = Vector3D.Vector3D(5,7,9);
        c = a.crossProd(b);
        d = Vector3D.Vector3D(-3, 6, -3);
        self.assertEquals(c,d)

        # perpindicular
        a = Vector3D.Vector3D(1,0,0);
        b = Vector3D.Vector3D(0,1,0);
        c = a.crossProd(b);
        d = Vector3D.Vector3D(0,0,1);
        self.assertEquals(c,d)

        # parallel
        a = Vector3D.Vector3D(1,2,3);
        b = a.crossProd(a);
        c = Vector3D.Vector3D(0,0,0);
        self.assertEquals(b,c)



    

suite = unittest.TestLoader().loadTestsFromTestCase(Vector3DTests)
unittest.TextTestRunner(verbosity=2).run(suite)
