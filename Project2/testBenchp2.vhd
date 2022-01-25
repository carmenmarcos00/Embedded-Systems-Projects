--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   18:27:41 11/20/2021
-- Design Name:   
-- Module Name:   C:/Users/cmarc/Desktop/UNIVERSIDAD/4/PRIMER CUATRI/p1_embebidos/testBenchp2.vhd
-- Project Name:  p1_embebidos
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: display_numbers
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY testBenchp2 IS
END testBenchp2;
 
ARCHITECTURE behavior OF testBenchp2 IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT display_numbers
    PORT(
         D : IN  std_logic_vector(3 downto 0);
         C : IN  std_logic_vector(3 downto 0);
         S : OUT  std_logic_vector(7 downto 0);
         AN : OUT  std_logic_vector(3 downto 0)
        );
    END COMPONENT;
    

   --Inputs
   signal D : std_logic_vector(3 downto 0) := (others => '0');
   signal C : std_logic_vector(3 downto 0) := (others => '0');

 	--Outputs
   signal S : std_logic_vector(7 downto 0);
   signal AN : std_logic_vector(3 downto 0);
   -- No clocks detected in port list. Replace <clock> below with 
   -- appropriate port name 
 
   constant clock_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: display_numbers PORT MAP (
          D => D,
          C => C,
          S => S,
          AN => AN
        );

   -- Clock process definitions
   clock_process :process
   begin
		--clock <= '0';
		wait for clock_period/2;
		--clock <= '1';
		wait for clock_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
      -- hold reset state for 100 ns.
      wait for 100 ns;

      -- insert stimulus here 
		 wait for 100 ns;	
		 C<="1000";
		 D<="0001";
		 wait for 100 ns;	
		 C<="0100";
		 D<="1000";
		 wait for 100 ns;	
		 C<="0010";
		 D<="0011";
      wait;
   end process;

END;
