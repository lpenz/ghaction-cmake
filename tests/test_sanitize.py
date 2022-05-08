import imp
import unittest

entrypoint = imp.load_source("entrypoint", "./entrypoint")


class SanitizeTests(unittest.TestCase):

    def test_clang(self):
        uut = entrypoint.Entrypoint({
            "INPUT_PRESET": "clang-sanitize-address",
        })
        uut.parse()
        self.assertEqual(
            uut.cmd_cmake,
            'cmake .  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ '
            '-DCMAKE_C_FLAGS="-fno-omit-frame-pointer -fsanitize=address" '
            '-DCMAKE_CXX_FLAGS="-fno-omit-frame-pointer -fsanitize=address"'
        )

    def test_gcc(self):
        uut = entrypoint.Entrypoint({
            "INPUT_PRESET": "gcc-sanitize-address",
        })
        uut.parse()
        self.assertEqual(
            uut.cmd_cmake,
            'cmake .  -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ '
            '-DCMAKE_C_FLAGS="-fno-omit-frame-pointer -fsanitize=address" '
            '-DCMAKE_CXX_FLAGS="-fno-omit-frame-pointer -fsanitize=address"'
        )
