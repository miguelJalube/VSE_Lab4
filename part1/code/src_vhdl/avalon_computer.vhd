-------------------------------------------------------------------------------
-- HEIG-VD, Haute Ecole d'Ingenierie et de Gestion du canton de Vaud
-- Institut REDS, Reconfigurable & Embedded Digital Systems
--
-- Fichier      : avalon_computer.vhd
--
-- Description  : Sequential calculator on an avalon MM slave
--
-- Auteur       : L. Fournier
-- Date         : 19.08.2022
-- Version      : 1.0
--
-- Utilisé dans : Laboratoire de VSE
--
--| Modifications |------------------------------------------------------------
-- Version   Auteur      Date               Description
-- 1.0       LFR         see header         First version.
-- 1.1       LFR         13.10.2022         Correct behavior readdatavalid
-------------------------------------------------------------------------------

--| Library |------------------------------------------------------------------
library ieee;
    use ieee.std_logic_1164.all;
    use ieee.numeric_std.all;
-------------------------------------------------------------------------------

--| Entity |-------------------------------------------------------------------
entity avalon_computer is
    generic (
        N        : integer range 0 to 32 := 3;
        ADDRSIZE : integer range 3 to 16 := 3;
        DATASIZE : integer range 1 to 16 := 16;
        ERRNO    : integer range 0 to 10  := 0
    );
    port (
        clk_i           : in  std_logic;
        rst_i           : in  std_logic;
        address_i       : in  std_logic_vector(ADDRSIZE-1 downto 0);
        byteenable_i    : in  std_logic_vector(1 downto 0);
        read_i          : in  std_logic;
        write_i         : in  std_logic;
        waitrequest_o   : out std_logic;
        readdatavalid_o : out std_logic;
        readdata_o      : out std_logic_vector(15 downto 0);
        writedata_i     : in  std_logic_vector(15 downto 0)
    );
end avalon_computer;
-------------------------------------------------------------------------------

`protect begin_protected
`protect version = 1
`protect encrypt_agent = "QuestaSim" , encrypt_agent_info = "2020.1_1"
`protect key_keyowner = "Mentor Graphics Corporation" , key_keyname = "MGC-VERIF-SIM-RSA-2"
`protect key_method = "rsa"
`protect encoding = ( enctype = "base64" , line_length = 64 , bytes = 256 )
`protect key_block
Kv4EoUu/ylLlIWjJjzESVrLb6XdSdyUWT7blzLxEzMj4vRD2Ca61Ljd7ZzqS3fiJ
aBI7FOcrEjwDqkwlQXorU/250moP9D1GSqf+dMFTmV2EGQbDD40rBfGPZOh6UjBK
VdT8oMzhOk+XBea6h30VBi4K99VdeNbZAGdZQAEZ3Lx9WkZrEL0xlzGZZogzAaT4
5vfpJTBODJSAZFS5T+9cVH7YYPCM9AaPMyUDJpsfVnCaDoe7DL1jkwvLhpZu4UUH
yHnhGgaLYcpLK0AEmV64H76kaGWqBclpXypKbmNC0nI76xdSYpWYmHnxOnR22bi+
toeMes8wmFgAwMPqQEUsPg==
`protect data_method = "aes128-cbc"
`protect encoding = ( enctype = "base64" , line_length = 64 , bytes = 19520 )
`protect data_block
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
`protect end_protected