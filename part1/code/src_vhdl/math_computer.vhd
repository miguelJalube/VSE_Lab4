library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity math_computer is
    generic (
        N        : integer := 3;
        DATASIZE : integer := 8;
        ERRNO    : integer := 0
        );
    port (
        clk_i    : in  std_logic;
        rst_i    : in  std_logic;
        a_i      : in  std_logic_vector(DATASIZE - 1 downto 0);
        b_i      : in  std_logic_vector(DATASIZE - 1 downto 0);
        c_i      : in  std_logic_vector(DATASIZE - 1 downto 0);
        valid_i  : in  std_logic;
        ready_o  : out std_logic;
        result_o : out std_logic_vector(DATASIZE - 1 downto 0);
        ready_i  : in  std_logic;
        valid_o  : out std_logic
        );
end math_computer;


`protect begin_protected
`protect version = 1
`protect encrypt_agent = "QuestaSim" , encrypt_agent_info = "2020.1_1"
`protect key_keyowner = "Mentor Graphics Corporation" , key_keyname = "MGC-VERIF-SIM-RSA-2"
`protect key_method = "rsa"
`protect encoding = ( enctype = "base64" , line_length = 64 , bytes = 256 )
`protect key_block
HfkONElDXnN5jkl73if737MtEvvY+2LatbWo12ZkzyEjDiqoFPplt1PfCTrDi93E
/Rj+mpC60AZ27p1RozqxLXYZf90JeHx5ked0utfNKVQFeQcKFDNpjnF2psdp0L6B
8HESoz1BSAnc9qmDQB9vOK8ybHfEHCJXtJ/1Lbe237UrO8huZCcljFHBYpaYk64V
9DGeUcOqE7U/bMaG7ZKElSrH6811s7uwOOqGJyhGh975BguzEyKLTM58NQrDEQML
4aEhMWHeLsb53rYxo8spRHOMFS4148Ylgis8/K3ADKTT2f0WoRGTjMA16wKXEJjl
fQIwlUx97L98qllJxxXERg==
`protect data_method = "aes128-cbc"
`protect encoding = ( enctype = "base64" , line_length = 64 , bytes = 6496 )
`protect data_block
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
2l5vEGLyIayIaDQD5NvGJw==
`protect end_protected
