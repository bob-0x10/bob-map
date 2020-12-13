create table if not exists node
(
	node_id int not null
		primary key,
	mac_addr varchar(255) not null,
	room_id int not null,
	x float default 0 not null,
	y float default 0 not null
)
charset=utf8mb4;

create table if not exists personal
(
	personal_id int auto_increment
		primary key,
	name varchar(255) not null,
	id varchar(255) not null,
	password varchar(255) null
)
charset=utf8mb4;

create table if not exists device
(
	device_id int auto_increment
		primary key,
	personal_id int not null,
	mac_addr varchar(255) not null,
	device_index int not null,
	constraint mac_addr
		unique (mac_addr),
	constraint device_ibfk_1
		foreign key (personal_id) references personal (personal_id)
)
charset=utf8mb4;

create table if not exists attendance
(
	`index` int auto_increment
		primary key,
	device_id int not null,
	personal_id int not null,
	room_id int not null,
	enter_time timestamp default CURRENT_TIMESTAMP not null,
	exit_time timestamp default CURRENT_TIMESTAMP not null,
	constraint attendance_ibfk_1
		foreign key (personal_id) references personal (personal_id),
	constraint attendance_ibfk_2
		foreign key (device_id) references device (device_id)
)
charset=utf8mb4;

create index personal_id
	on attendance (personal_id);

create index personal_id
	on device (personal_id);

create table if not exists log
(
	`index` int auto_increment
		primary key,
	att_processed tinyint(1) default 0 not null,
	pos_processed tinyint(1) default 0 not null,
	node_id int not null,
	device_id int not null,
	RSSI tinyint not null,
	time timestamp default CURRENT_TIMESTAMP not null,
	constraint log_ibfk_1
		foreign key (node_id) references node (node_id),
	constraint log_ibfk_2
		foreign key (device_id) references device (device_id)
)
charset=utf8mb4;

create index device_id
	on log (device_id);

create index node_id
	on log (node_id);

create table if not exists position
(
	`index` int auto_increment
		primary key,
	device_id int not null,
	personal_id int not null,
	x float not null,
	y float not null,
	time timestamp default CURRENT_TIMESTAMP not null,
	constraint position_ibfk_1
		foreign key (personal_id) references personal (personal_id),
	constraint position_ibfk_2
		foreign key (device_id) references device (device_id)
)
charset=utf8mb4;

create index personal_id
	on position (personal_id);

