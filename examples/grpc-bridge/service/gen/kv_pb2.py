# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: kv.proto

import sys
_b = sys.version_info[0] < 3 and (lambda x: x) or (lambda x: x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()

DESCRIPTOR = _descriptor.FileDescriptor(
    name='kv.proto',
    package='kv',
    syntax='proto3',
    serialized_pb=_b(
        '\n\x08kv.proto\x12\x02kv\"\x19\n\nGetRequest\x12\x0b\n\x03key\x18\x01 \x01(\t\"\x1c\n\x0bGetResponse\x12\r\n\x05value\x18\x01 \x01(\t\"(\n\nSetRequest\x12\x0b\n\x03key\x18\x01 \x01(\t\x12\r\n\x05value\x18\x02 \x01(\t\"\x19\n\x0bSetResponse\x12\n\n\x02ok\x18\x01 \x01(\x08\x32T\n\x02KV\x12&\n\x03Get\x12\x0e.kv.GetRequest\x1a\x0f.kv.GetResponse\x12&\n\x03Set\x12\x0e.kv.SetRequest\x1a\x0f.kv.SetResponseb\x06proto3'
    ))

_GETREQUEST = _descriptor.Descriptor(
    name='GetRequest',
    full_name='kv.GetRequest',
    filename=None,
    file=DESCRIPTOR,
    containing_type=None,
    fields=[
        _descriptor.FieldDescriptor(
            name='key',
            full_name='kv.GetRequest.key',
            index=0,
            number=1,
            type=9,
            cpp_type=9,
            label=1,
            has_default_value=False,
            default_value=_b("").decode('utf-8'),
            message_type=None,
            enum_type=None,
            containing_type=None,
            is_extension=False,
            extension_scope=None,
            options=None,
            file=DESCRIPTOR),
    ],
    extensions=[],
    nested_types=[],
    enum_types=[],
    options=None,
    is_extendable=False,
    syntax='proto3',
    extension_ranges=[],
    oneofs=[],
    serialized_start=16,
    serialized_end=41,
)

_GETRESPONSE = _descriptor.Descriptor(
    name='GetResponse',
    full_name='kv.GetResponse',
    filename=None,
    file=DESCRIPTOR,
    containing_type=None,
    fields=[
        _descriptor.FieldDescriptor(
            name='value',
            full_name='kv.GetResponse.value',
            index=0,
            number=1,
            type=9,
            cpp_type=9,
            label=1,
            has_default_value=False,
            default_value=_b("").decode('utf-8'),
            message_type=None,
            enum_type=None,
            containing_type=None,
            is_extension=False,
            extension_scope=None,
            options=None,
            file=DESCRIPTOR),
    ],
    extensions=[],
    nested_types=[],
    enum_types=[],
    options=None,
    is_extendable=False,
    syntax='proto3',
    extension_ranges=[],
    oneofs=[],
    serialized_start=43,
    serialized_end=71,
)

_SETREQUEST = _descriptor.Descriptor(
    name='SetRequest',
    full_name='kv.SetRequest',
    filename=None,
    file=DESCRIPTOR,
    containing_type=None,
    fields=[
        _descriptor.FieldDescriptor(
            name='key',
            full_name='kv.SetRequest.key',
            index=0,
            number=1,
            type=9,
            cpp_type=9,
            label=1,
            has_default_value=False,
            default_value=_b("").decode('utf-8'),
            message_type=None,
            enum_type=None,
            containing_type=None,
            is_extension=False,
            extension_scope=None,
            options=None,
            file=DESCRIPTOR),
        _descriptor.FieldDescriptor(
            name='value',
            full_name='kv.SetRequest.value',
            index=1,
            number=2,
            type=9,
            cpp_type=9,
            label=1,
            has_default_value=False,
            default_value=_b("").decode('utf-8'),
            message_type=None,
            enum_type=None,
            containing_type=None,
            is_extension=False,
            extension_scope=None,
            options=None,
            file=DESCRIPTOR),
    ],
    extensions=[],
    nested_types=[],
    enum_types=[],
    options=None,
    is_extendable=False,
    syntax='proto3',
    extension_ranges=[],
    oneofs=[],
    serialized_start=73,
    serialized_end=113,
)

_SETRESPONSE = _descriptor.Descriptor(
    name='SetResponse',
    full_name='kv.SetResponse',
    filename=None,
    file=DESCRIPTOR,
    containing_type=None,
    fields=[
        _descriptor.FieldDescriptor(
            name='ok',
            full_name='kv.SetResponse.ok',
            index=0,
            number=1,
            type=8,
            cpp_type=7,
            label=1,
            has_default_value=False,
            default_value=False,
            message_type=None,
            enum_type=None,
            containing_type=None,
            is_extension=False,
            extension_scope=None,
            options=None,
            file=DESCRIPTOR),
    ],
    extensions=[],
    nested_types=[],
    enum_types=[],
    options=None,
    is_extendable=False,
    syntax='proto3',
    extension_ranges=[],
    oneofs=[],
    serialized_start=115,
    serialized_end=140,
)

DESCRIPTOR.message_types_by_name['GetRequest'] = _GETREQUEST
DESCRIPTOR.message_types_by_name['GetResponse'] = _GETRESPONSE
DESCRIPTOR.message_types_by_name['SetRequest'] = _SETREQUEST
DESCRIPTOR.message_types_by_name['SetResponse'] = _SETRESPONSE
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

GetRequest = _reflection.GeneratedProtocolMessageType(
    'GetRequest',
    (_message.Message,),
    dict(
        DESCRIPTOR=_GETREQUEST,
        __module__='kv_pb2'
        # @@protoc_insertion_point(class_scope:kv.GetRequest)
    ))
_sym_db.RegisterMessage(GetRequest)

GetResponse = _reflection.GeneratedProtocolMessageType(
    'GetResponse',
    (_message.Message,),
    dict(
        DESCRIPTOR=_GETRESPONSE,
        __module__='kv_pb2'
        # @@protoc_insertion_point(class_scope:kv.GetResponse)
    ))
_sym_db.RegisterMessage(GetResponse)

SetRequest = _reflection.GeneratedProtocolMessageType(
    'SetRequest',
    (_message.Message,),
    dict(
        DESCRIPTOR=_SETREQUEST,
        __module__='kv_pb2'
        # @@protoc_insertion_point(class_scope:kv.SetRequest)
    ))
_sym_db.RegisterMessage(SetRequest)

SetResponse = _reflection.GeneratedProtocolMessageType(
    'SetResponse',
    (_message.Message,),
    dict(
        DESCRIPTOR=_SETRESPONSE,
        __module__='kv_pb2'
        # @@protoc_insertion_point(class_scope:kv.SetResponse)
    ))
_sym_db.RegisterMessage(SetResponse)

_KV = _descriptor.ServiceDescriptor(
    name='KV',
    full_name='kv.KV',
    file=DESCRIPTOR,
    index=0,
    options=None,
    serialized_start=142,
    serialized_end=226,
    methods=[
        _descriptor.MethodDescriptor(
            name='Get',
            full_name='kv.KV.Get',
            index=0,
            containing_service=None,
            input_type=_GETREQUEST,
            output_type=_GETRESPONSE,
            options=None,
        ),
        _descriptor.MethodDescriptor(
            name='Set',
            full_name='kv.KV.Set',
            index=1,
            containing_service=None,
            input_type=_SETREQUEST,
            output_type=_SETRESPONSE,
            options=None,
        ),
    ])
_sym_db.RegisterServiceDescriptor(_KV)

DESCRIPTOR.services_by_name['KV'] = _KV

# @@protoc_insertion_point(module_scope)
