<script>
	import { createEventDispatcher } from 'svelte'
	
	const dispatch = createEventDispatcher()

	function visibleChanged(e) {
		if (e.target.visible) {
			dispatch('open')
		} else {
			dispatch('close')
		}
	}

	async function getImages() {
		const res = await fetch('http://localhost:8000/images')

		const images = await res.json();

		if (res.ok) {
			return images;
		} else {
			throw new Error(images);
		}
	}

	$: allImages = getImages();
</script>

<kor-modal visible={true} id="modal" on:visible-changed={visibleChanged} width="80%" height="80%" icon="image" label="Select Image">
	<kor-table>
		<kor-table-row slot="header">
			<kor-table-cell head grid-cols="6">Start</kor-table-cell>
			<kor-table-cell head grid-cols="18">Name</kor-table-cell>
		</kor-table-row>
		{#await allImages then images}
			{#each images as image}
				<kor-table-row on:click={() => dispatch('select', image.name)}>
					<kor-table-cell grid-cols="6">
						<kor-image src="http://localhost:8000{image.path}" fit="cover" />
					</kor-table-cell>
					<kor-table-cell grid-cols="18">{image.name}</kor-table-cell>
				</kor-table-row>
			{/each}
		{/await}
	</kor-table>
</kor-modal>
