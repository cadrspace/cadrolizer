env = Environment();

env.Append(CPPPATH = [
    '/home/avp/src/dist/iotivity-0.9.0/resource/include/',
    '/home/avp/src/dist/iotivity-0.9.0/resource/csdk/stack/include',
    '/home/avp/src/dist/iotivity-0.9.0/resource/csdk/ocsocket/include',
    '/home/avp/src/dist/iotivity-0.9.0/resource/csdk/ocrandom/include',
    '/home/avp/src/dist/iotivity-0.9.0/resource/csdk/logger/include',
    '/home/avp/src/dist/iotivity-0.9.0/resource/csdk/libcoap',
    '/home/avp/src/dist/iotivity-0.9.0/resource/oc_logger/include'
])

env.Append(CXXFLAGS = ['-std=c++0x', '-Wall'])
env.Prepend(LIBPATH = [ '/home/avp/src/dist/iotivity-0.9.0/resource/release/obj',
                        '/home/avp/src/dist/iotivity-0.9.0/resource/csdk/linux/release',
                        '/home/avp/src/dist/iotivity-0.9.0/resource/oc_logger/lib'
])
env.Append(LIBS = ['oc', 'octbstack', 'oc_logger', '-lpthread'])

cadrolizer = env.Program('cadrolizer', [ 'cadrolizer.cpp', 'CadrolizerResource.cpp'])
cz         = env.Program('cz', 'cz.cpp')