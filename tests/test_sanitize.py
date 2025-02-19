import importlib.machinery
import types
import unittest

loader = importlib.machinery.SourceFileLoader("entrypoint", "./entrypoint")
entrypoint = types.ModuleType(loader.name)
loader.exec_module(entrypoint)


class SanitizeTests(unittest.TestCase):

    def test_clang(self):
        uut = entrypoint.Entrypoint(
            {
                "INPUT_PRESET": "clang-sanitize-address",
            }
        )
        uut.parse()
        self.assertEqual(
            uut.cmd_cmake,
            "cmake .  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ "
            '-DCMAKE_C_FLAGS="-fno-omit-frame-pointer -fsanitize=address" '
            '-DCMAKE_CXX_FLAGS="-fno-omit-frame-pointer -fsanitize=address"',
        )

    def test_gcc(self):
        uut = entrypoint.Entrypoint(
            {
                "INPUT_PRESET": "gcc-sanitize-address",
            }
        )
        uut.parse()
        self.assertEqual(
            uut.cmd_cmake,
            "cmake .  -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ "
            '-DCMAKE_C_FLAGS="-fno-omit-frame-pointer -fsanitize=address" '
            '-DCMAKE_CXX_FLAGS="-fno-omit-frame-pointer -fsanitize=address"',
        )
