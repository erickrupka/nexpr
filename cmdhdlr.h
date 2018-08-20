typedef struct CMD_OBJ {
	char	name[80];
	int	code;
	char	(*func)();
} cmd_obj_type, *cmd_obj_ptr_type;



typedef struct CMD_TBL {
	char 			name[80];
	int  			code;
	cmd_obj_ptr_type	obj_ptr;
	int			*num_objs;
} cmd_tbl_type, *cmd_tbl_ptr_type;

