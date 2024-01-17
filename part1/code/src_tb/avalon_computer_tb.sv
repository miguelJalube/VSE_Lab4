

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
        $display("Starting a write command");
        write_i = 1;
        address_i = address;
        byteenable_i = byteenable;
        writedata_i = data;

        wait(waitrequest_o == 0);
        @(posedge clk_i);
        write_i = 0;

    endtask

    task avalon_read(int unsigned address, int unsigned byteenable, output int unsigned data);
        $display("Starting a read command");
        read_i = 1;
        address_i = address;
        byteenable_i = byteenable;

        wait(readdatavalid_o == 1);
        data = readdata_o;
        @(posedge clk_i);
        
        read_i = 0;
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


    // Make C function visible to SystemVerilog code
    import "DPI-C" context task CTask();

    initial begin
        fork
            init_signals();
            apply_reset();
        join

        @(posedge clk_i);
        @(posedge clk_i);

        if (0) begin
            automatic int unsigned data;
            avalon_write(0, 3, 5);
            avalon_read(0, 3, data);
            if (data != 5) begin
                $error("Aie aie aie");
            end
        end

        CTask();
        $finish();
    end

endmodule
