/*
 * Copyright (C) 2016 Bryan Callahan <bryancallahan@gmail.com>
 * Released under the GPL version 2 only.
 * 
 * Linux Driver to Mock RCIN. For more information, please see:
 * https://github.com/emlid/navio-rcio-linux-driver/blob/master/rcio_rcin.c
 * https://stuff.mit.edu/afs/sipb/contrib/linux/samples/kobject/kobject-example.c
 */
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>

#define RCIO_RCIN_MAX_CHANNELS 16

static u16 measurements[RCIO_RCIN_MAX_CHANNELS] = {0};

static ssize_t channel_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    int value = -1;

    int channel;

    if (sscanf(attr->attr.name, "ch%d", &channel) < 0) {
        return -EIO;
    }

    value = measurements[channel];

    if (value < 0) {
        return value;
    }

    return sprintf(buf, "%d\n", value);
}

static ssize_t channel_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    int channel;
    if (sscanf(attr->attr.name, "ch%d", &channel) < 0) {
        return -EIO;
    }

    u16 val;
    if (kstrtou16(buf, 10, &val) < 0) {
        return -EIO;
    }
    measurements[channel] = val;

    return count;
}

#define RCIN_CHANNEL_ATTR(channel) __ATTR(channel, 0664, channel_show, channel_store)

static struct kobj_attribute ch0_attribute = RCIN_CHANNEL_ATTR(ch0);
static struct kobj_attribute ch1_attribute = RCIN_CHANNEL_ATTR(ch1);
static struct kobj_attribute ch2_attribute = RCIN_CHANNEL_ATTR(ch2);
static struct kobj_attribute ch3_attribute = RCIN_CHANNEL_ATTR(ch3);
static struct kobj_attribute ch4_attribute = RCIN_CHANNEL_ATTR(ch4);
static struct kobj_attribute ch5_attribute = RCIN_CHANNEL_ATTR(ch5);
static struct kobj_attribute ch6_attribute = RCIN_CHANNEL_ATTR(ch6);
static struct kobj_attribute ch7_attribute = RCIN_CHANNEL_ATTR(ch7);
static struct kobj_attribute ch8_attribute = RCIN_CHANNEL_ATTR(ch8);
static struct kobj_attribute ch9_attribute = RCIN_CHANNEL_ATTR(ch9);
static struct kobj_attribute ch10_attribute = RCIN_CHANNEL_ATTR(ch10);
static struct kobj_attribute ch11_attribute = RCIN_CHANNEL_ATTR(ch11);
static struct kobj_attribute ch12_attribute = RCIN_CHANNEL_ATTR(ch12);
static struct kobj_attribute ch13_attribute = RCIN_CHANNEL_ATTR(ch13);
static struct kobj_attribute ch14_attribute = RCIN_CHANNEL_ATTR(ch14);
static struct kobj_attribute ch15_attribute = RCIN_CHANNEL_ATTR(ch15);

static bool connected;

static ssize_t connected_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", connected ? 1 : 0);
}

static ssize_t connected_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    int val;
    if (kstrtoint(buf, 10, &val) < 0) {
        return -EIO;
    }
    connected = (val == 1);

    return count;
}

static struct kobj_attribute connected_attribute = __ATTR(connected, 0664, connected_show, connected_store);

// Create a list of attributes (end with NULL to terminate the list)...
// https://github.com/emlid/navio-rcio-linux-driver/blob/master/rcio_rcin.c#L61
static struct attribute *attrs[] = {
    &ch0_attribute.attr,
    &ch1_attribute.attr,
    &ch2_attribute.attr,
    &ch3_attribute.attr,
    &ch4_attribute.attr,
    &ch5_attribute.attr,
    &ch6_attribute.attr,
    &ch7_attribute.attr,
    &ch8_attribute.attr,
    &ch9_attribute.attr,
    &ch10_attribute.attr,
    &ch11_attribute.attr,
    &ch12_attribute.attr,
    &ch13_attribute.attr,
    &ch14_attribute.attr,
    &ch15_attribute.attr,
    &connected_attribute.attr,
    NULL,
};

static struct attribute_group attr_group = {
    .name = "rcin",
    .attrs = attrs,
};

static struct kobject *rcio_mock;

static int __init rcin_mock_init(void)
{
    int retval;

    rcio_mock = kobject_create_and_add("rcio_mock", kernel_kobj);
    if (!rcio_mock) {
        return -ENOMEM;
    }

    retval = sysfs_create_group(rcio_mock, &attr_group);
    if (retval) {
        kobject_put(rcio_mock);
    }

    return retval;
}

static void __exit rcin_mock_exit(void)
{
    kobject_put(rcio_mock);
}

module_init(rcin_mock_init);
module_exit(rcin_mock_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Bryan Callahan <bryancallahan@gmail.com>");
