import random
import subprocess
import unittest
import os

DATA_SOURCE = "../validation/"
OUTPUT_DIR = "../out/"

class FeistelTester(unittest.TestCase):

    def test_hexa_format(self):
        """
        This test checks all hexa Bytes lines in all output txt files in OUTPUT_DIR
        """
        for output_file in sorted(os.listdir(OUTPUT_DIR)):
            if output_file.endswith("_hexoutput.txt"):
                with open(os.path.join(OUTPUT_DIR, output_file), "r") as fr:
                    lines = fr.readlines()
                assert len(lines) == 3
                for line in lines:
                    assert len(bytes.fromhex(line.strip())) <= 100
                # check whether line 1 and 3 are the same
                print(lines)
                assert lines[0].strip() == lines[2].strip()[:len(lines[0].strip())]

    def test_encoded_bin_files(self):
        """
        This test compares first 100 Bytes of all .bin files in OUTPUT_DIR contra Bytes in Hexa format
        """
        for encoded_file in sorted(os.listdir(OUTPUT_DIR)):
            if encoded_file.endswith(".bin"):
                with open(os.path.join(OUTPUT_DIR, encoded_file), "rb") as fr:
                    encoded_bytes = fr.read()

                # let's find an appropriate output file in hexa format
                for output_file in sorted(os.listdir(OUTPUT_DIR)):
                    if output_file.endswith("_hexoutput.txt") and encoded_file.replace(".bin", "") in output_file:
                        with open(os.path.join(OUTPUT_DIR, output_file), "r") as fr:
                            lines = fr.readlines()
                        encoded_hexa_bytes = bytes.fromhex(lines[1].strip())
                        assert encoded_bytes[:100] == encoded_hexa_bytes