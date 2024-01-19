/*
HEIG-VD
2024-01-19

Leandro SARAIVA MAIA
Miguel JALUBE

Labo 4 VSE
Partie 1
*/

module avalon_computer_tb#(int N=3, int ADDRSIZE=3, int DATASIZE=16, int ERRNO=0);

    logic clk_i = 0;
    logic rst_i;
    logic[ADDRSIZE-1:0] address_i;
    logic[1:0] byteenable_i;
    logic read_i;
    logic write_i;
    logic waitrequest_o;
    logic readdatavalid_o;
    logic[15:0] readdata_o;
    logic[15:0] writedata_i;

    default clocking cb @(posedge clk_i);
    endclocking

    always #5 clk_i = ~clk_i;

    avalon_computer#(N, ADDRSIZE, DATASIZE, ERRNO) duv(.*);

    export "DPI-C" task avalon_write;
    export "DPI-C" task avalon_read;

    task avalon_write(int unsigned address, int unsigned byteenable, int unsigned data);
        $display("[SV] write %05d at 0x%05x   time:%t", data, address, $time);
        write_i = 1;
        address_i = address;
        byteenable_i = byteenable;
        writedata_i = data;

        wait(waitrequest_o == 0);
        @(posedge clk_i);
        write_i = 0;

    endtask

    task avalon_read(int unsigned address, int unsigned byteenable, output int unsigned data);
        read_i = 1;
        address_i = address;
        byteenable_i = byteenable;

        @(posedge clk_i);

        wait(readdatavalid_o == 1);
        data = readdata_o;
        read_i = 0;
        $display("[SV] read  %05d at 0x%05x   time:%t", data, address, $time);
        wait(readdatavalid_o == 0);
    endtask

    task init_signals();
        write_i = 0;
        read_i = 0;
        address_i = 0;
        byteenable_i = 0;
        writedata_i = 0;
        @(posedge clk_i);
    endtask

    task apply_reset();
        rst_i = 1;
        #20;
        rst_i = 0;
    endtask

    int unsigned i = 0;

    task watchdog(int unsigned timeout = 100);
        // if x cycles without read or write, end simulation
        // reset timeout if read or write

        while(1) begin
            @(posedge clk_i);
            i++;
            if(!($stable(read_i) && $stable(write_i))) begin
                i = 0;
            end;
            if(i > timeout) begin
                $display("ouaf at %t ns", $time);
                $stop();
            end;
        end;
    endtask


    // Make C function visible to SystemVerilog code
    import "DPI-C" context task CTask();

    initial begin
        fork
            init_signals();
            apply_reset();
        join

        // Start chien de garde at the same time as
        // test suite so when an error occurs, the
        // chien de garde will stop the program
        fork
            watchdog();
            CTask();
        join_any

        $display("====================================");
        $display("Test bench ended gracefully at %t ns", $time);
        $display("====================================");
        $stop();
    end

endmodule
