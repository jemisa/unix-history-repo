/*-
 * Copyright (c) 2013 The FreeBSD Foundation
 * All rights reserved.
 *
 * This software was developed by Aleksandr Rybalko under sponsorship from the
 * FreeBSD Foundation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD$
 */

/* Generic framebuffer */
/* TODO unlink from VT(9) */
/* TODO done normal /dev/fb methods */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/bus.h>
#include <sys/conf.h>
#include <sys/kernel.h>
#include <sys/malloc.h>
#include <sys/module.h>
#include <sys/queue.h>
#include <sys/fbio.h>

#include <machine/bus.h>

#include <dev/vt/vt.h>
#include <dev/vt/hw/fb/vt_fb.h>

#include "fb_if.h"

LIST_HEAD(fb_list_head_t, fb_list_entry) fb_list_head =
    LIST_HEAD_INITIALIZER(fb_list_head);
struct fb_list_entry {
	struct fb_info	*fb_info;
	struct cdev	*fb_si;
	LIST_ENTRY(fb_list_entry) fb_list;
};

struct fbd_softc {
	device_t	sc_dev;
	struct fb_info	*sc_info;
};

static void fbd_evh_init(void *);
/* SI_ORDER_SECOND, just after EVENTHANDLERs initialized. */
SYSINIT(fbd_evh_init, SI_SUB_CONFIGURE, SI_ORDER_SECOND, fbd_evh_init, NULL);

static d_open_t		fb_open;
static d_close_t	fb_close;
static d_read_t		fb_read;
static d_write_t	fb_write;
static d_ioctl_t	fb_ioctl;
static d_mmap_t		fb_mmap;

static struct cdevsw fb_cdevsw = {
	.d_version =	D_VERSION,
	.d_flags =	D_NEEDGIANT,
	.d_open =	fb_open,
	.d_close =	fb_close,
	.d_read =	fb_read,
	.d_write =	fb_write,
	.d_ioctl =	fb_ioctl,
	.d_mmap =	fb_mmap,
	.d_name =	"fb",
};

static int framebuffer_dev_unit = 0;

static int
fb_open(struct cdev *dev, int oflags, int devtype, struct thread *td)
{

	return (0);
}

static int
fb_close(struct cdev *dev, int fflag, int devtype, struct thread *td)
{

	return (0);
}

static int
fb_ioctl(struct cdev *dev, u_long cmd, caddr_t data, int fflag,
    struct thread *td)
{

	return (0);
}

static int
fb_read(struct cdev *dev, struct uio *uio, int ioflag)
{

	return (0); /* XXX nothing to read, yet */
}

static int
fb_write(struct cdev *dev, struct uio *uio, int ioflag)
{

	return (0); /* XXX nothing written */
}

static int
fb_mmap(struct cdev *dev, vm_ooffset_t offset, vm_paddr_t *paddr, int nprot,
    vm_memattr_t *memattr)
{
	struct fb_info *info;

	info = dev->si_drv1;
	if (offset < info->fb_size) {
		*paddr = info->fb_pbase + offset;
		return (0);
	}
	return (EINVAL);
}


static void
vt_fb_mem_wr1(struct fb_info *sc, uint32_t o, uint8_t v)
{

	KASSERT((o < sc->fb_size), ("Offset %#08x out of fb size", o));
	*(uint8_t *)(sc->fb_vbase + o) = v;
}

static void
vt_fb_mem_wr2(struct fb_info *sc, uint32_t o, uint16_t v)
{

	KASSERT((o < sc->fb_size), ("Offset %#08x out of fb size", o));
	*(uint16_t *)(sc->fb_vbase + o) = v;
}

static void
vt_fb_mem_wr4(struct fb_info *sc, uint32_t o, uint32_t v)
{

	KASSERT((o < sc->fb_size), ("Offset %#08x out of fb size", o));
	*(uint32_t *)(sc->fb_vbase + o) = v;
}

static void
vt_fb_mem_copy(struct fb_info *sc, uint32_t offset_to, uint32_t offset_from,
    uint32_t size)
{

	memmove((void *)(sc->fb_vbase + offset_to), (void *)(sc->fb_vbase +
	    offset_from), size);
}

static void
vt_fb_indir_wr1(struct fb_info *sc, uint32_t o, uint8_t v)
{

	KASSERT((o < sc->fb_size), ("Offset %#08x out of fb size", o));
	sc->fb_write(sc->fb_priv, o, &v, 1);
}

static void
vt_fb_indir_wr2(struct fb_info *sc, uint32_t o, uint16_t v)
{

	KASSERT((o < sc->fb_size), ("Offset %#08x out of fb size", o));
	sc->fb_write(sc->fb_priv, o, &v, 2);
}

static void
vt_fb_indir_wr4(struct fb_info *sc, uint32_t o, uint32_t v)
{

	KASSERT((o < sc->fb_size), ("Offset %#08x out of fb size", o));
	sc->fb_write(sc->fb_priv, o, &v, 4);
}

static void
vt_fb_indir_copy(struct fb_info *sc, uint32_t offset_to, uint32_t offset_from,
    uint32_t size)
{

	sc->copy(sc->fb_priv, offset_to, offset_from, size);
}

int
fb_probe(struct fb_info *info)
{

	if (info->fb_size == 0)
		return (ENXIO);

	if (info->fb_write != NULL) {
		if (info->fb_write == NULL) {
			return (EINVAL);
		}
		info->fb_flags |= FB_FLAG_NOMMAP;
		info->wr1 = &vt_fb_indir_wr1;
		info->wr2 = &vt_fb_indir_wr2;
		info->wr4 = &vt_fb_indir_wr4;
		info->copy = &vt_fb_indir_copy;
	} else if (info->fb_vbase != 0) {
		if (info->fb_pbase == 0)
			info->fb_flags |= FB_FLAG_NOMMAP;
		info->wr1 = &vt_fb_mem_wr1;
		info->wr2 = &vt_fb_mem_wr2;
		info->wr4 = &vt_fb_mem_wr4;
		info->copy = &vt_fb_mem_copy;
	} else
		return (ENXIO);

	return (0);
}


static int
fb_init(struct fb_list_entry *entry, int unit)
{
	struct fb_info *info;

	info = entry->fb_info;
	entry->fb_si = make_dev(&fb_cdevsw, unit, UID_ROOT, GID_WHEEL,
	    0600, "fb%d", unit);
	entry->fb_si->si_drv1 = info;

	return (0);
}

int
fbd_list()
{
	struct fb_list_entry *entry;

	if (LIST_EMPTY(&fb_list_head))
		return (ENOENT);

	LIST_FOREACH(entry, &fb_list_head, fb_list) {
		printf("FB %s @%p\n", entry->fb_info->fb_name,
		    (void *)entry->fb_info->fb_pbase);
	}

	return (0);
}

static struct fb_list_entry *
fbd_find(struct fb_info* info)
{
	struct fb_list_entry *entry, *tmp;

	LIST_FOREACH_SAFE(entry, &fb_list_head, fb_list, tmp) {
		if (entry->fb_info == info) {
			return (entry);
		}
	}

	return (NULL);
}

int
fbd_register(struct fb_info* info)
{
	struct fb_list_entry *entry;
	int err, first;

	first = 0;
	if (LIST_EMPTY(&fb_list_head))
		first++;

	entry = fbd_find(info);
	if (entry != NULL) {
		/* XXX Update framebuffer params */
		return (0);
	}

	err = fb_probe(info);
	if (err)
		return (err);

	entry = malloc(sizeof(struct fb_list_entry), M_DEVBUF, M_WAITOK|M_ZERO);
	entry->fb_info = info;

	LIST_INSERT_HEAD(&fb_list_head, entry, fb_list);

	err = fb_init(entry, framebuffer_dev_unit++);
	if (err)
		return (err);

	if (first)
		vt_fb_attach(info);

	return (0);
}

int
fbd_unregister(struct fb_info* info)
{
	struct fb_list_entry *entry, *tmp;

	LIST_FOREACH_SAFE(entry, &fb_list_head, fb_list, tmp) {
		if (entry->fb_info == info) {
			LIST_REMOVE(entry, fb_list);
			free(entry, M_DEVBUF);
			return (0);
		}
	}

	return (ENOENT);
}

static void
register_fb_wrap(void *arg, void *ptr)
{

	fbd_register((struct fb_info *)ptr);
}

static void
unregister_fb_wrap(void *arg, void *ptr)
{

	fbd_unregister((struct fb_info *)ptr);
}

static void
fbd_evh_init(void *ctx)
{

	EVENTHANDLER_REGISTER(register_framebuffer, register_fb_wrap, NULL,
	    EVENTHANDLER_PRI_ANY);
	EVENTHANDLER_REGISTER(unregister_framebuffer, unregister_fb_wrap, NULL,
	    EVENTHANDLER_PRI_ANY);
}

/* Newbus methods. */
static int
fbd_probe(device_t dev)
{

	return (BUS_PROBE_NOWILDCARD);
}

static int
fbd_attach(device_t dev)
{
	struct fbd_softc *sc;
	int err;

	sc = device_get_softc(dev);

	sc->sc_dev = dev;
	sc->sc_info = FB_GETINFO(device_get_parent(dev));
	if (sc->sc_info == NULL)
		return (ENXIO);
	err = fbd_register(sc->sc_info);

	return (err);
}

static int
fbd_detach(device_t dev)
{
	struct fbd_softc *sc;
	int err;

	sc = device_get_softc(dev);

	err = fbd_unregister(sc->sc_info);

	return (err);
}


static device_method_t fbd_methods[] = {
	/* Device interface */
	DEVMETHOD(device_probe,		fbd_probe),
	DEVMETHOD(device_attach,	fbd_attach),
	DEVMETHOD(device_detach,	fbd_detach),

	DEVMETHOD(device_shutdown,	bus_generic_shutdown),
	DEVMETHOD(device_suspend,	bus_generic_suspend),
	DEVMETHOD(device_resume,	bus_generic_resume),

	{ 0, 0 }
};

driver_t fbd_driver = {
	"fbd",
	fbd_methods,
	sizeof(struct fbd_softc)
};

devclass_t	fbd_devclass;

DRIVER_MODULE(fbd, fb, fbd_driver, fbd_devclass, 0, 0);
DRIVER_MODULE(fbd, drmn, fbd_driver, fbd_devclass, 0, 0);
MODULE_VERSION(fbd, 1);

